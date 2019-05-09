#ifndef CPLEX_STRUCTS_HPP_
#define CPLEX_STRUCTS_HPP_


#include <ilcplex/cplex.h>
#include <ilcplex/ilocplex.h>

/** \var typedef SemiContVarMatrix
 * \brief an array of arrays of semicontinuous constraintSet variables
 */
typedef IloArray<IloSemiContVarArray>   SemiContVarMatrix;
/** \var typedef NumExprArgArray
 * \brief an array of real valued expression arguments
 */
typedef IloArray<IloNumExprArg>    NumExprArgArray;
/** \var typedef NumExprArgMatrix
 * \brief an array of arrays of real valued expression arguments
 */
typedef IloArray<NumExprArgArray>  NumExprArgMatrix;

/** \var typedef NumExprMatrix
 * \brief an array of arrays of real valued expressions
 */
typedef IloArray<IloNumExprArray> NumExprMatrix;
typedef IloArray<NumExprMatrix>   NumExpr3D;

/** \var typedef ExprMatrix
 * \brief an array of arrays of expressions
 */
typedef IloArray<IloExprArray>     ExprMatrix;
/** \var typedef IntArrayMatrix
 * \brief an array of arrays of integer valued constraintSet constants
 */
typedef IloArray<IloIntArray>      IntArrayMatrix;
/** \var typedef IntVarMatrix
 * \brief an array of arrays of integer valued constraintSet variables
 */
typedef IloArray<IloIntVarArray>   IntVarMatrix;
/** \var typedef NumVarMatrix
 * \brief an array of arrays of real valued constraintSet variables
 */
typedef IloArray<IloNumVarArray>   NumVarMatrix;
/** \var typedef NumVarMatrix
 * \brief an array of arrays of real valued constraintSet variables
 */
typedef IloArray<NumVarMatrix>     NumVar3D;
/** \var typedef BoolVarMatrix
 * \brief an array of arrays of boolean valued constraintSet variables
 */
typedef IloArray<IloBoolVarArray>  BoolVarMatrix;
/** \var typedef BoolVarMatrix
 * \brief an array of arrays of boolean valued constraintSet variables
 */
typedef IloArray<BoolVarMatrix>    BoolVar3D;
/** \var typedef NumMatrix
 * \brief an array of arrays of real valued constraintSet constants
 */
typedef IloArray<IloNumArray>      NumMatrix;
/** \var typedef IntMatrix
 * \brief an array of arrays of integer valued constraintSet constants
 */
typedef IloArray<IloIntArray>      IntMatrix;
/** \var typedef IntExprMatrix
 * \brief an array of arrays of integer valued expressions
 */
typedef IloArray<IloIntExprArray>  IntExprMatrix;
/** \var typedef IntExprMatrix
 * \brief an array of arrays of integer valued expressions
 */
typedef IloArray<IntExprMatrix>    IntExpr3D;
/** \var typedef IfThenArray
 * \brief an array of conditional expression objects
 */
typedef IloArray<IloIfThen>        IfThenArray;
/** \var typedef IfThenMatrix
 * \brief an array of arrays of conditional expression objects
 */
typedef IloArray<IfThenArray>      IfThenMatrix;
/** \var typedef IfThenMatrix
 * \brief an array of arrays of conditional expression objects
 */
typedef IloArray<IfThenMatrix>     IfThen3D;

#endif /* CPLEX_STRUCTS_HPP_ */
