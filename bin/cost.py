#!/usr/bin/python
# vim: set fileencoding=utf-8 :

import csv

def usage():
    print "Usage: cost.py [COMMAND] [FILE]..."
    print ""
    print "commands:"
    command_format = "    %-14s %-s"
    print command_format % ("summary"   , "summarize cost and emissions from the schedule files"   )
    print command_format % ("categories", "show cost and emission data for each generator category")
    print command_format % ("details"   , "show details of inputs, outputs, costs, and emissions"  )

def main(arguments):
    if 2 > len(arguments):
        usage()
        return 1
    command = arguments[0]
    schedule_filenames = arguments[1:]

    zones = parse_csv_zones_file_with_name("zone.csv")
    generators = parse_csv_generators_file_with_name("generators.csv", zones)
    schedules_by_file = []
    for filename in schedule_filenames:
        current_schedule = parse_csv_schedule_file_with_name(filename, generators)
        schedules_by_file.append((filename, current_schedule))
    schedule = PeriodStatus(reduce(lambda x,y: x+y, map(lambda pair: pair[1].step_statuses, schedules_by_file)))

    if "summary" == command:
        for filename, current_schedule in schedules_by_file:
            print 'Total Cost for %s: %12.2f' % (filename, current_schedule.cost())
        print 'Total cost for all periods: %.2f' % schedule.cost()
        print 'Total co2 emitted: %.2f short tons' % (schedule.co2_pounds_emitted() / 2000.)
        print 'Total sox emitted: %.2f short tons' % (schedule.sox_pounds_emitted() / 2000.)
        print 'Total nox emitted: %.2f short tons' % (schedule.nox_pounds_emitted() / 2000.)
    elif "categories" == command:
        print table_as_csv(*table_of_cost_and_generation_by_category(schedule, generators))
    elif "details" == command:
        print table_as_csv(*status_table(schedule))
    else:
        usage()
        return 1
    return 0

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return "(" + str(self.x) + ";" + str(self.y) + ")"

class Interval:
    def __init__(self, min, max, closed_left, closed_right):
        self.min = min
        self.max = max
        self.closed_left = closed_left
        self.closed_right = closed_right
    def contains(self, x, epsilon):
        return (
            (    self.closed_left and self.min <= x + epsilon and (self.closed_right and x - epsilon <= self.max or not self.closed_right and x - epsilon < self.max)) or
            (not self.closed_left and self.min <  x + epsilon and (self.closed_right and x - epsilon <= self.max or not self.closed_right and x - epsilon < self.max))
        )
    def __str__(self):
        return (self.closed_left and '[' or '(') + str(self.min) + ',' + str(self.max) + (self.closed_right and ']' or ')')

class Segment:
    def __init__(self, p0, p1, closed_left, closed_right):
        self.p0 = p0
        self.p1 = p1
        self._range = Interval(p0.x, p1.x, closed_left, closed_right)
        self._max_x = p1.x
    def contains(self, x, epsilon=0):
        return self._range.contains(x,epsilon)
    def value(self, x, epsilon=0): 
        if self.contains(x, epsilon):
            if abs(self.p0.x - x) < epsilon:
                return self.p0.y
            try:
                y = self.p0.y + (x - self.p0.x) * (self.p1.y - self.p0.y) / float(self.p1.x - self.p0.x)
            except:
                print x, epsilon, self
                raise
            return y
        raise Exception("Value not in segment: " + str(x))
    def __str__(self):
        return str(self.p0) + ";" + str(self.p1)
    def max_x(self):
        return self._max_x
    def range(self):
        return self._range

class IntervalUnion:
    def __init__(self, intervals):
        self.intervals = intervals
    def contains(self, x):
        for interval in self.intervals:
            if interval.contains(x):
                return True
        return False
    def __str__(self):
        return ' ∪ '.join([str(interval) for interval in self.intervals])

class Line:
    def __init__(self, p0, p1):
        if p0.x == p1.x:
            raise Exception('Vertical or degenerate line defined with ' + str(p0) + ' and ' + str(p1))
        self.slope = (p1.y-p0.y)/(p1.x-p0.x)
        self.y_intercept = p0.y - self.slope * p0.x
    def value_at(self, x):
        return self.slope * x + self.y_intercept

class Piecewise:
    def __init__(self, segments, epsilon=1e-1):
        self.epsilon = epsilon
        self.segments = segments
    def value_at(self, x):
        for segment in self.segments:
            if segment.contains(x, epsilon=self.epsilon):
                return segment.value(x,epsilon=self.epsilon);
        raise Exception("Value not in any segments: %s for piecewise %s" % (str(x), self))
    def is_defined_at(self, x):
        for segment in self.segments:
            if segment.contains(x, epsilon=self.epsilon):
                return True
        return False
    def range(self):
        return IntervalUnion([segment.range() for segment in self.segments])
    def extrapolate_value_at(self, x):
        for segment in self.segments:
            if segment.contains(x, epsilon=self.epsilon):
                return segment.value(x,epsilon=self.epsilon);
        if x > self.max_x():
            return Line(self.segments[-1].p0, self.segments[-1].p1).value_at(x)
        previousSegment = self.segments[0]
        for nextSegment in self.segments[1:]:
            artificialSegment = Segment(previousSegment.p1, nextSegment.p0, True, False)
            if artificialSegment.contains(x):
                return artificialSegment.value(x)
            previousSegment = nextSegment
        raise Exception('extrapolation only defined between intervals ' + str(self.range()) + ' and asked for value ' + str(x))
    def __str__(self):
        return str([str(segment) for segment in self.segments])
    def max_x(self):
        return self.segments[-1].max_x()

class Zone:
    def __init__(self, name, co2_cost_per_ton, sox_cost_per_ton, nox_cost_per_ton):
        self.name = name
        self.co2_cost_per_ton = co2_cost_per_ton
        self.sox_cost_per_ton = sox_cost_per_ton
        self.nox_cost_per_ton = nox_cost_per_ton
        self.generators = []

class Generator:
    def __init__(self, name, zone, io_curve, start_cost, stop_cost, fuel_cost, category, co2_pounds_emitted_per_mmbtu_fuel_input, sox_pounds_emitted_per_mmbtu_fuel_input, nox_pounds_emitted_per_mmbtu_fuel_input, operations_and_maintenance_cost_per_MWh, operations_and_maintenance_cost_per_hour):
        self.name                                     = name
        self.io_curve                                 = Piecewise([Segment(Point(-1e-3,0), Point(1e-3,0), True, False)] + io_curve.segments)
        self.start_cost                               = start_cost
        self.stop_cost                                = stop_cost
        self.fuel_cost                                = fuel_cost
        self.category                                 = category
        self.co2_pounds_emitted_per_mmbtu_fuel_input  = co2_pounds_emitted_per_mmbtu_fuel_input
        self.sox_pounds_emitted_per_mmbtu_fuel_input  = sox_pounds_emitted_per_mmbtu_fuel_input
        self.nox_pounds_emitted_per_mmbtu_fuel_input  = nox_pounds_emitted_per_mmbtu_fuel_input
        self.zone                                     = zone
        self.operations_and_maintenance_cost_per_MWh  = operations_and_maintenance_cost_per_MWh
        self.operations_and_maintenance_cost_per_hour = operations_and_maintenance_cost_per_hour
        zone.generators.append(self)
    def __str__(self):
        return "" + str(self.io_curve) + " " + str(self.start_cost) + " " + str(self.stop_cost) + " " + str(self.fuel_cost) + " " + str(self.category)
    def fuel_mmbtu_used_at(self, power_output):
        if self.io_curve.is_defined_at(power_output):
            return self.io_curve.value_at(power_output)
        print 'Warning: power output outside range:\t' + str(power_output) + '\t∉\t' + str(self.io_curve.range()) + '\t' + self.name
        return self.io_curve.extrapolate_value_at(power_output)
    def power_cost(self, power_output):
        return self.fuel_mmbtu_used_at(power_output) * self.fuel_cost
    def co2_pounds_emitted(self, power_output):
        return self.fuel_mmbtu_used_at(power_output) * self.co2_pounds_emitted_per_mmbtu_fuel_input;
    def co2_emissions_cost(self, power_output):
        return self.co2_pounds_emitted(power_output) * self.zone.co2_cost_per_ton / 2000.
    def sox_pounds_emitted(self, power_output):
        return self.fuel_mmbtu_used_at(power_output) * self.sox_pounds_emitted_per_mmbtu_fuel_input;
    def sox_emissions_cost(self, power_output):
        return self.sox_pounds_emitted(power_output) * self.zone.sox_cost_per_ton / 2000.
    def nox_pounds_emitted(self, power_output):
        return self.fuel_mmbtu_used_at(power_output) * self.nox_pounds_emitted_per_mmbtu_fuel_input;
    def nox_emissions_cost(self, power_output):
        return self.nox_pounds_emitted(power_output) * self.zone.nox_cost_per_ton / 2000.
    def operations_and_maintenance_variable_cost(self, power_output):
        return self.operations_and_maintenance_cost_per_MWh * power_output
    def operations_and_maintenance_fixed_cost(self):
        return self.operations_and_maintenance_cost_per_hour

class GeneratorStatus:
    def __init__(self, generator, power, start, stop):
        self.generator = generator
        self.power     = power
        self.start     = start
        self.stop      = stop
    def start_cost(self):
        return self.generator.start_cost * self.start
    def stop_cost(self):
        return self.generator.stop_cost * self.stop
    def fuel_cost(self):
        return self.generator.power_cost(self.power)
    def co2_pounds_emitted(self):
        return self.generator.co2_pounds_emitted(self.power)
    def co2_emissions_cost(self):
        return self.generator.co2_emissions_cost(self.power)
    def sox_pounds_emitted(self):
        return self.generator.sox_pounds_emitted(self.power)
    def sox_emissions_cost(self):
        return self.generator.sox_emissions_cost(self.power)
    def nox_pounds_emitted(self):
        return self.generator.nox_pounds_emitted(self.power)
    def nox_emissions_cost(self):
        return self.generator.nox_emissions_cost(self.power)
    def operations_and_maintenance_variable_cost(self):
        return self.generator.operations_and_maintenance_variable_cost(self.power)
    def operations_and_maintenance_fixed_cost(self):
        return self.generator.operations_and_maintenance_fixed_cost() * 1 # fixed cost per hour times 1 hour
    def fuel_used_mmbtu(self):
        return self.generator.fuel_mmbtu_used_at(self.power)
    def cost(self):
        return (
            self.start_cost() +
            self.stop_cost() +
            self.fuel_cost() +
            self.co2_emissions_cost() +
            self.sox_emissions_cost() +
            self.nox_emissions_cost() +
            self.operations_and_maintenance_variable_cost() +
            self.operations_and_maintenance_fixed_cost()
        )
    def __str__(self):
        return " ".join([str(self.power), ["","START"][self.start], ["","STOP"][self.stop]])

class StepStatus:
    def __init__(self, generator_statuses):
        self.generator_statuses = generator_statuses
    def cost(self):
        return sum(map(lambda x: x.cost(), self.generator_statuses))
    def power(self):
        return sum(map(lambda x: x.power, self.generator_statuses))
    def co2_pounds_emitted(self):
        return sum(map(lambda x: x.co2_pounds_emitted(), self.generator_statuses))
    def sox_pounds_emitted(self):
        return sum(map(lambda x: x.sox_pounds_emitted(), self.generator_statuses))
    def nox_pounds_emitted(self):
        return sum(map(lambda x: x.nox_pounds_emitted(), self.generator_statuses))

class PeriodStatus:
    def __init__(self, step_statuses):
        self.step_statuses = step_statuses
    def cost(self):
        step_costs = [step_status.cost() for step_status in self.step_statuses]
        period_cost = sum(step_costs)
        return period_cost
    def co2_pounds_emitted(self):
        co2_pounds_emitted_per_step = map(lambda step: step.co2_pounds_emitted(), self.step_statuses)
        return reduce(lambda x,y: x+y, co2_pounds_emitted_per_step)
    def sox_pounds_emitted(self):
        sox_pounds_emitted_per_step = map(lambda step: step.sox_pounds_emitted(), self.step_statuses)
        return reduce(lambda x,y: x+y, sox_pounds_emitted_per_step)
    def nox_pounds_emitted(self):
        nox_pounds_emitted_per_step = map(lambda step: step.nox_pounds_emitted(), self.step_statuses)
        return reduce(lambda x,y: x+y, nox_pounds_emitted_per_step)
    def append(self, other_period_status):
        self.step_statuses += other_period_status.step_statuses

def find_field_name(dict_reader, possible_fieldnames):
    possible_fieldnames_lowercase = [str.lower() for str in possible_fieldnames]
    for current_fieldname in dict_reader.fieldnames:
        if current_fieldname.lower() in possible_fieldnames_lowercase:
            return current_fieldname

def parse_csv_zones_file_with_name(filename):
    stream = open(filename)
    reader = csv.DictReader(stream, delimiter=',', quotechar='"')
    zones = []
    for row in reader:
        name = row['Zone']
        co2_cost_per_ton = float(row['CO2 Cost'])
        sox_cost_per_ton = float(row['Sox Cost'])
        nox_cost_per_ton = float(row['Nox Cost'])
        zone = Zone(name, co2_cost_per_ton, sox_cost_per_ton, nox_cost_per_ton)
        zones.append(zone)
    stream.close()
    return zones

def parse_csv_generators_file_with_name(generators_filename, zones):
    generators_file = open(generators_filename)
    generators_reader = csv.DictReader(generators_file, delimiter=',', quotechar='"')
    
    generators = []
    for generator_row in generators_reader:
        io_curve_field_name = find_field_name(generators_reader, [
            'Input-Output curve (Power output in MW;Fuel input in MMBtu/hr)',
            'Input-Output curve (Power output in MW; Fuel input in MMBtu/hr)',
            'Input-Output curve (Power output; Fuel input)'
        ])
        fuel_cost_field_name = find_field_name(generators_reader, ['Fuel Cost', 'Fuel Cost ($/MMBtu)'])
        co2_pounds_emitted_per_mmbtu_fuel_input_field_name = find_field_name(generators_reader, ['CO2 rate', 'CO2 rate (lbs/MMBtu)', 'CO2 emissions rate (lb/MMBtu)'])
        sox_pounds_emitted_per_mmbtu_fuel_input_field_name = find_field_name(generators_reader, ['SOx rate', 'SOx rate (lbs/MMBtu)', 'SOx emissions rate (lb/MMBtu)'])
        nox_pounds_emitted_per_mmbtu_fuel_input_field_name = find_field_name(generators_reader, ['NOx rate', 'NOx rate (lbs/MMBtu)', 'NOx emissions rate (lb/MMBtu)'])

        io_curve_value = generator_row[io_curve_field_name]
        point_strings = io_curve_value[1:][:-1].split(");(")
        point_string_pairs = [point.split(";") for point in point_strings]
        io_curve_points= [Point(float(point[0]), float(point[1])) for point in point_string_pairs]
        point_pairs = zip(io_curve_points[0::2], io_curve_points[1::2])
        segments = [Segment(p0, p1, True, False) for p0, p1 in point_pairs]
        name                                     = generator_row['Name']
        io_curve                                 = Piecewise(segments)
        stop_cost                                = float(generator_row["shutdown cost (K$)"]) * 1000
        start_cost                               = float(generator_row["startup cost (K$)"]) * 1000
        fuel_cost                                = float(generator_row[fuel_cost_field_name])
        category                                 = generator_row.get('Category','UNKNOWN_CATEGORY')
        zone_index                               = int(generator_row['Zone'])
        zone                                     = zones[zone_index]
        co2_pounds_emitted_per_mmbtu_fuel_input  = float(generator_row[co2_pounds_emitted_per_mmbtu_fuel_input_field_name])
        sox_pounds_emitted_per_mmbtu_fuel_input  = float(generator_row[sox_pounds_emitted_per_mmbtu_fuel_input_field_name])
        nox_pounds_emitted_per_mmbtu_fuel_input  = float(generator_row[nox_pounds_emitted_per_mmbtu_fuel_input_field_name])
        operations_and_maintenance_cost_per_MWh  = float(generator_row.get('Variable O&M ($/MWh)') or '0') # optional column with default 0
        operations_and_maintenance_cost_per_hour = float(generator_row.get('Fixed O&M ($/kW-Yr)') or '0') * io_curve.max_x() * 1000 / ( 365.25 * 24 )
        generator                                = Generator(name, zone, io_curve, start_cost, stop_cost, fuel_cost, category, co2_pounds_emitted_per_mmbtu_fuel_input, sox_pounds_emitted_per_mmbtu_fuel_input, nox_pounds_emitted_per_mmbtu_fuel_input, operations_and_maintenance_cost_per_MWh, operations_and_maintenance_cost_per_hour)
        generators.append(generator)
    generators_file.close()
    return generators

def parse_csv_schedule_file_with_name(schedule_filename, generators, epsilon=0):
    schedule_file = open(schedule_filename)
    reader = csv.reader(schedule_file, delimiter=',', quotechar='"')
    reader.next()
    reader.next()
    costs = []
    step_statuses = []
    for row in reader:
        index = 0
        fuel_cost  = 0
        start_cost = 0
        stop_cost  = 0
        generator_statuses = []
        for generator in generators:
            power = float(row[index+1])
            isOn = power > epsilon
            if 0 == len(step_statuses):
                wasOn = False
            else:
                previousStepStatus = step_statuses[-1]
                wasOn = previousStepStatus.generator_statuses[index].power > epsilon
            start = isOn and not(wasOn)
            stop = wasOn and not(isOn)
            #start = "SW_ON_STATE" == row[index+len(generators)*3+2]
            #stop = "SW_OFF_STATE" == row[index+len(generators)*3+2]
            generator_status = GeneratorStatus(generator, power, start, stop)
            generator_statuses.append(generator_status)
            index = index+1
        step_status = StepStatus(generator_statuses)
        step_statuses.append(step_status)
    period_status = PeriodStatus(step_statuses)
    schedule_file.close()
    return period_status

def generator_unique_categories_accumulator(x, generator):
    category = generator.category
    if not type(x) is list:
        return generator_unique_categories_accumulator(generator_unique_categories_accumulator([],x),generator)
    if not category in x:
        return x + [category]
    return x

def table_as_csv(headers, table):
    from StringIO import StringIO
    stream = StringIO()
    stream.write(",".join(headers))
    stream.write("\n")
    csv_writer = csv.DictWriter(stream,headers)
    csv_writer.writerows(table)
    return stream.getvalue()

def table_of_cost_and_generation_by_category(schedule, generators):
    categories = reduce(generator_unique_categories_accumulator, generators)
    all_category_data = []
    step = 0
    headers = []
    headers.append('step')
    for category in categories:
        headers.append(category+' generation')
    headers.append('total generation')
    for category in categories:
        headers.append(category+' cost')
    headers.append('total cost')
    step == 0
    for step_status in schedule.step_statuses:
        step += 1
        category_data = {}
        for category in categories:
            category_data['step'] = step
            category_data[category+' cost'] = 0
            category_data[category+' generation'] = 0
            category_data['total cost'] = 0
            category_data['total generation'] = 0
        for generator_status in step_status.generator_statuses:
            category_data[generator_status.generator.category+' cost'] += generator_status.cost()
            category_data[generator_status.generator.category+' generation'] += generator_status.power
            category_data['total cost'] += generator_status.cost()
            category_data['total generation'] += generator_status.power
        all_category_data.append(category_data)
    return headers, all_category_data

def status_table(schedule):
    table = []
    step_index = -1
    for step in schedule.step_statuses:
        step_index = step_index + 1
        tuples = [
            ( 'step'                       , step_index                ),
            ( 'step power (MW)'            , step.power()              ),
            ( 'step co2 emissions (pounds)', step.co2_pounds_emitted() ),
            ( 'step sox emissions (pounds)', step.sox_pounds_emitted() ),
            ( 'step nox emissions (pounds)', step.nox_pounds_emitted() ),
            ( 'step cost ($)'              , step.cost()               )
        ]
        for generator_status in step.generator_statuses:
            tuples += [
                ( generator_status.generator.name + ' power (MW)'             , generator_status.power                                     ),
                ( generator_status.generator.name + ' start'                  , generator_status.start                                     ),
                ( generator_status.generator.name + ' stop'                   , generator_status.stop                                      ),
                ( generator_status.generator.name + ' fuel input (MMBtu)'     , generator_status.fuel_used_mmbtu()                         ),
                ( generator_status.generator.name + ' co2 emissions (pounds)' , generator_status.co2_pounds_emitted()                      ),
                ( generator_status.generator.name + ' sox emissions (pounds)' , generator_status.sox_pounds_emitted()                      ),
                ( generator_status.generator.name + ' nox emissions (pounds)' , generator_status.nox_pounds_emitted()                      ),
                ( generator_status.generator.name + ' fuel cost ($)'          , generator_status.fuel_cost()                               ),
                ( generator_status.generator.name + ' co2 cost ($)'           , generator_status.co2_emissions_cost()                      ),
                ( generator_status.generator.name + ' sox cost ($)'           , generator_status.sox_emissions_cost()                      ),
                ( generator_status.generator.name + ' nox cost ($)'           , generator_status.nox_emissions_cost()                      ),
                ( generator_status.generator.name + ' start cost ($)'         , generator_status.start_cost()                              ),
                ( generator_status.generator.name + ' stop cost ($)'          , generator_status.stop_cost()                               ),
                ( generator_status.generator.name + ' o&m variable cost ($)'  , generator_status.operations_and_maintenance_variable_cost()),
                ( generator_status.generator.name + ' o&m fixed cost ($)'     , generator_status.operations_and_maintenance_fixed_cost()   ),
                ( generator_status.generator.name + ' cost ($)'               , generator_status.cost()                                    ) 
            ]
        table.append(tuples)
    table.append([
        ( 'step'                       , 'TOTAL'                       ),
        ( 'step co2 emissions (pounds)', schedule.co2_pounds_emitted() ),
        ( 'step sox emissions (pounds)', schedule.sox_pounds_emitted() ),
        ( 'step nox emissions (pounds)', schedule.nox_pounds_emitted() ),
        ( 'step cost ($)'              , schedule.cost()               )
    ])
    keys = [x for x, y in table[0]]
    dictionaries = [dict((x,y) for x,y in row) for row in table]
    return keys, dictionaries

def print_costs(costs):
    import itertools
    width = 10
    float_format = "%" + str(width) + ".2f"
    string_format = "%" + str(width) + "s"
    cost_format = "%5s " + " ".join(itertools.repeat(float_format, 1+len(generators)))
    header_format = cost_format.replace(float_format,string_format)
    header_arguments = ["step"] + ["gen" + str(x) for x in range(len(generators))] + ["total"]
    print header_format%tuple(header_arguments)
    for index, cost in enumerate(costs):
        print cost_format%tuple([index] + cost)
    totals = [sum(column) for column in zip(*costs)]
    print cost_format%tuple(["total"] + totals)

if __name__ == "__main__":
    import sys
    exit_code = main(sys.argv[1:])
    sys.exit(exit_code)











