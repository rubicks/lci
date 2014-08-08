/**
 * Structures and functions for parsing tokens to a parse tree.  The parser
 * reads through a list of tokens (generated by the tokenizer) and adds semantic
 * meaning to them by forming them into a parse tree which can then be passed on
 * to later stages (such as the interpreter).
 *
 * \file   parser.h
 *
 * \author Justin J. Meza
 *
 * \date   2010-2012
 */

/**
 * \page impvar The Implicit Variable
 *
 * The implicit variable in LOLCODE is denoted by the keyword \c IT and stores a
 * copy of the result of the most recently evaluated expression statement (an
 * expression all by itself on a line).  (See
 * http://lolcode.com/specs/1.2#conditionals for an example.)
 */

/**
 * \page lolebnf The LOLCODE EBNF
 *
 * Presented below is the EBNF (see
 * http://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form) for the
 * LOLCODE language that \c lci parses.  Note that by this stage, the scanner
 * has:
 *   - already removed any whitespace between tokens,
 *   - added in and truncated newline tokens at logical line breaks, and
 *   - added an end-of-file (\c $) token.
 *
 * \section progebnf Program Structure
 *
 * These production rules dictate the overall form of the program.
 *
 * \par
 * MainNode ::= \c TT_HAI \a version \c TT_NEWLINE BlockNode \c $
 *
 * \par
 * BlockNode ::= StmtNode *
 *
 * \section typesebnf Types
 *
 * These production rules specify some general types of parse structures.
 *
 * \par
 * ConstantNode ::= Boolean | Integer | Float | String
 *
 * \par
 * IdentifierNode ::= Identifier | \c TT_SRS ExprNode
 *
 * \par
 * TypeNode ::= \c TT_NOOB | \c TT_TROOF | \c TT_NUMBR | \c TT_NUMBAR | \c
 * TT_YARN
 *
 * \section stmtebnf Statements
 *
 * These production rules specify the types of statements formed.
 *
 * \par
 * StmtNode ::= CastStmtNode | PrintStmtNode | InputStmtNode |
 * AssignmentStmtNode | DeclarationStmtNode | IfThenElseStmtNode |
 * SwitchStmtNode | BreakStmt | ReturnStmtNode | LoopStmtNode |
 * DeallocationStmtNode | FuncDefStmtNode | ExprStmt | AltArrayDefStmtNode
 *
 * \par
 * CastStmtNode ::= IdentifierNode \c TT_ISNOWA TypeNode \c TT_NEWLINE
 *
 * \par
 * PrintStmtNode ::= \c TT_VISIBLE ExprNodeList \c TT_BANG ? \c TT_NEWLINE
 *
 * \par
 * InputStmtNode ::= \c TT_GIMMEH IdentifierNode \c TT_NEWLINE
 *
 * \par
 * AssignmentStmtNode ::= IdentifierNode \c TT_R ExprNode \c TT_NEWLINE
 *
 * \par
 * DeclarationStmtNode ::= IdentifierNode \c TT_HASA IdentifierNode
 * Initialization ? \c TT_NEWLINE
 *
 * \par
 * Initialization ::= \c TT_ITZ ExprNode | \c TT_ITZA TypeNode | \c TT_ITZLIEKA IdentifierNode
 *
 * \par
 * IfThenElseStmtNode ::= \c TT_ORLY \c TT_NEWLINE \c TT_YARLY \c TT_NEWLINE
 * BlockNode ElseIf * Else ? \c TT_OIC \c TT_NEWLINE
 *
 * \par
 * ElseIf ::= \c TT_MEBBE ExprNode \c TT_NEWLINE BlockNode
 *
 * \par
 * Else ::= \c TT_NOWAI \c TT_NEWLINE BlockNode
 *
 * \par
 * SwitchStmtNode ::= \c TT_WTF \c TT_NEWLINE Case + DefaultCase ? \c TT_OIC \c
 * TT_NEWLINE
 *
 * \par
 * Case ::= \c TT_OMG ExprNode \c TT_NEWLINE BlockNode
 *
 * \par
 * DefaultCase ::= \c TT_OMGWTF \c TT_NEWLINE BlockNode
 *
 * \par
 * BreakStmt ::= \c TT_GTFO \c TT_NEWLINE
 *
 * \par
 * ReturnStmtNode ::= \c TT_FOUNDYR ExprNode \c TT_NEWLINE
 *
 * \par
 * LoopStmtNode ::= \c TT_IMINYR IdentifierNode LoopUpdate ? LoopGuard ? \c
 * TT_NEWLINE BlockNode \c TT_IMOUTTAYR IdentifierNode \c TT_NEWLINE
 *
 * \par
 * LoopUpdate ::= LoopUpdateOp \c TT_YR IdentifierNode
 *
 * \par
 * LoopUpdateOp ::= \c TT_UPPIN | \c TT_NERFIN | UnaryFunction
 *
 * \par
 * UnaryFunction ::= The name of a previously defined unary function.
 *
 * \par
 * LoopGuard ::= \c TT_TIL ExprNode | \c TT_WILE ExprNode
 *
 * \par
 * DeallocationStmtNode ::= IdentifierNode \c TT_RNOOB
 *
 * \par
 * FuncDefStmtNode ::= \c TT_HOWIZ IdentifierNode IdentifierNode FunctionArgs ?
 * \c TT_NEWLINE BlockNode \c TT_IFUSAYSO \c TT_NEWLINE
 *
 * \par
 * FunctionArgs ::= \c TT_YR IdentifierNode FunctionArg *
 *
 * \par
 * FunctionArg ::= \c TT_ANYR IdentifierNode
 *
 * \par
 * ExprStmt ::= ExprNode \c TT_NEWLINE
 *
 * \par
 * AltArrayDefStmtNode ::= \c TT_OHAIIM IdentifierNode AltArrayInheritance ? \c
 * TT_NEWLINE BlockNode \c TT_KTHX \c TT_NEWLINE
 *
 * \par
 * AltArrayInheritance ::= \c TT_IMLIEK IdentifierNode
 *
 * \section exprebnf Expressions
 *
 * These production rules specify the types of expressions formed.
 *
 * \par
 * ExprNode ::= CastExprNode | ConstantNode | IdentifierNode | FuncCallExprNode
 * | OpExprNode | ImplicitVariable
 *
 * \par
 * CastExprNode ::= \c TT_MAEK ExprNode \c TT_A TypeNode
 *
 * \par
 * FuncCallExprNode ::= IdentifierNode \c TT_IZ IdentifierNode FunctionArgs ?
 * TT_MKAY
 *
 * \par
 * OpExprNode ::= UnaryOp | BinaryOp | NaryOp
 *
 * \par
 * UnaryOp ::= UnaryOpType ExprNode
 *
 * \par
 * UnaryOpType ::= \c TT_NOT
 *
 * \par
 * BinaryOp ::= BinaryOpType ExprNode \c TT_AN ? ExprNode
 *
 * \par
 * BinaryOpType ::= \c TT_SUMOF | \c TT_DIFFOF | \c TT_PRODUKTOF | \c
 * TT_QUOSHUNTOF | \c TT_MODOF | \c BIGGROF | \c SMALLROF | \c TT_BOTHOF | \c
 * TT_EITHEROF | \c TT_WONOF
 *
 * \par
 * NaryOp ::= NaryOpType NaryOpArgs \c TT_MKAY
 *
 * \par
 * NaryOpType ::= \c TT_ALLOF | \c TT_ANYOF | \c TT_SMOOSH
 *
 * \par
 * NaryOpArgs ::= ExprNode NaryOpArg +
 *
 * \par
 * NaryOpArg ::= \c TT_AN ? ExprNode
 *
 * \par
 * ImplicitVariable ::= \c TT_IT
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "tokenizer.h"

#undef DEBUG

/**
 * Represents a statement type.
 */
typedef enum {
	ST_CAST,            /**< Cast statement. */
	ST_PRINT,           /**< Print statement. */
	ST_INPUT,           /**< Input statement. */
	ST_ASSIGNMENT,      /**< Assignment statement. */
	ST_DECLARATION,     /**< Declaration statement. */
	ST_IFTHENELSE,      /**< If/then/else statement. */
	ST_SWITCH,          /**< Switch statement. */
	ST_BREAK,           /**< Break statement. */
	ST_RETURN,          /**< Return statement. */
	ST_LOOP,            /**< Loop statement. */
	ST_DEALLOCATION,    /**< Deallocation statement. */
	ST_FUNCDEF,         /**< Function definition statement. */
	ST_EXPR,            /**< Expression statement. */
	ST_ALTARRAYDEF,     /**< Function definition statement. */
} StmtType;

/**
 * Stores statement data.
 */
typedef struct {
	StmtType type; /**< The type of statement in \a node. */
	void *stmt;    /**< The statement. */
} StmtNode;

/**
 * Stores a list of statements.
 */
typedef struct {
	unsigned int num; /**< The number of statements. */
	StmtNode **stmts; /**< The array of statements. */
} StmtNodeList;

/**
 * Represents an expression type.
 */
typedef enum {
	ET_CAST,       /**< Cast expression. */
	ET_CONSTANT,   /**< Constant expression. */
	ET_IDENTIFIER, /**< Identifier expression. */
	ET_FUNCCALL,   /**< Function call expression. */
	ET_OP,         /**< Operation expression. */
	ET_IMPVAR      /**< \ref impvar "Implicit variable". */
} ExprType;

/**
 * Stores an expression.
 */
typedef struct {
	ExprType type; /**< The type of expression in \a expr. */
	void *expr;    /**< The expression. */ 
} ExprNode;

/**
 * Stores a list of expressions.
 */
typedef struct {
	unsigned int num; /**< The number of expressions. */
	ExprNode **exprs; /**< The array of statements. */
} ExprNodeList;

/**
 * Represents an identifier type.
 */
typedef enum {
	IT_DIRECT,  /**< Direct identifier. */
	IT_INDIRECT /**< Indirect identifier. */
} IdentifierType;

/**
 * Stores an identifier.
 */
typedef struct identifiernode {
	IdentifierType type;         /**< The type of identifier in \a id. */
	void *id;                    /**< The identifier. */
	char *fname;                 /**< The original file name. */
	unsigned int line;           /**< The original line number. */
	struct identifiernode *slot; /**< The slot to access. */
} IdentifierNode;

/**
 * Stores a list of identifiers.
 */
typedef struct {
	unsigned int num;     /**< The number of identifiers. */
	IdentifierNode **ids; /**< The array of identifiers. */
} IdentifierNodeList;

/**
 * Stores a code block.
 */
typedef struct {
	StmtNodeList *stmts; /**< The list of statements in the block. */
} BlockNode;

/**
 * Stores a list of code blocks.
 */
typedef struct {
	unsigned int num;   /**< The number of code blocks. */
	BlockNode **blocks; /**< The array of code blocks. */
} BlockNodeList;

/**
 * Represents a constant type.
 */
typedef enum {
	CT_INTEGER, /**< Integer constant. */
	CT_FLOAT,   /**< Decimal constant. */
	CT_BOOLEAN, /**< Boolean constant. */
	CT_STRING,  /**< String constant. */
	CT_NIL,     /**< Nil constant. */
	CT_ARRAY    /**< Array constant. */
} ConstantType;

/**
 * Stores constant data.
 */
typedef union {
	long long int i; /**< Integer data. */
	float f;         /**< Decimal data. */
	char *s;         /**< String data. */
} ConstantData;

/**
 * Stores a constant.
 */
typedef struct {
	ConstantType type; /**< The type of constant in \a data. */
	ConstantData data; /**< The constant. */
} ConstantNode;

/**
 * Stores a function definition statement.
 */
typedef struct {
	IdentifierNode *scope;    /**< The scope of the function. */
	IdentifierNode *name;     /**< The name of the function. */
	IdentifierNodeList *args; /**< The names of the function arguments. */
	BlockNode *body;          /**< The body of the function. */
} FuncDefStmtNode;

/**
 * Stores an alternate array definition statement.
 */
typedef struct {
	IdentifierNode *name;   /**< The name of the array. */
	BlockNode *body;        /**< The body of the array definition. */
	IdentifierNode *parent; /**< An optional inherited array. */
} AltArrayDefStmtNode;

/**
 * Stores the main code block of a program.
 *
 * \note This could be represented with just a BlockNode, but it seems
 * significant enough to merit its own structure.
 */
typedef struct {
	BlockNode *block; /**< The first block of code to execute. */
} MainNode;

/**
 * Stores a variable type.
 */
typedef struct {
	ConstantType type; /**< The type variable. */
} TypeNode;

/**
 * Stores a cast statement.  This statement changes the type of a variable.
 */
typedef struct {
	IdentifierNode *target; /**< The name of the variable to cast. */
	TypeNode *newtype;      /**< The type to cast \a target to. */
} CastStmtNode;

/**
 * Stores a print statement.  This statement prints a list of expressions with
 * an optional newline.
 */
typedef struct {
	ExprNodeList *args; /**< The expressions to print. */
	int nonl;           /**< Whether to print an ending newline. */
} PrintStmtNode;

/**
 * Stores an input statement.  This statement accepts input from the user and
 * stores it in a variable.
 */
typedef struct {
	IdentifierNode *target; /**< The variable to store the input in. */
} InputStmtNode;

/**
 * Stores an assignment statement.  This statement stores an evaluated
 * expression in a variable.
 */
typedef struct {
	IdentifierNode *target; /**< The variable to store \a expr in. */
	ExprNode *expr;         /**< The expression to store. */
} AssignmentStmtNode;

/**
 * Stores a declaration statement.  This statement creates a new variable in a
 * given scope and optionally initializes it to an expression.
 *
 * \note Either provide \a expr OR \a type.  If both are provided, the result is
 * undefined.
 */
typedef struct {
	IdentifierNode *scope;  /**< The scope to create the variable in. */
	IdentifierNode *target; /**< The name of the variable to create. */
	ExprNode *expr;         /**< An optional initialization expression. */
	TypeNode *type;         /**< An optional initialization type. */
	IdentifierNode *parent; /**< An optional inherited array. */
} DeclarationStmtNode;

/**
 * Stores an if/then/else statement.  This statement checks the value of the
 * \ref impvar "implicit variable" and executes the \c yes block if it can be
 * cast to true.  Else, the \c guards are evaluated and the corresponding code
 * in one of the \c blocks is executed.  Finally, if none of these things occur,
 * the \c no block is executed.
 */
typedef struct {
	BlockNode *yes;        /**< The code to execute if \c IT is true. */
	BlockNode *no;         /**< The code to execute if nothing else does. */
	ExprNodeList *guards;  /**< The guards for the \c blocks. */
	BlockNodeList *blocks; /**< The code to execute if a guard is true. */
} IfThenElseStmtNode;

/**
 * Stores a switch statement.  This statement compares the value of the \ref
 * impvar "implicit variable" to each of the \a guards and executes the
 * respective block of code in \a blocks if they match.  If no matches are
 * found, the optional default block of code, \a def, is executed.
 */
typedef struct {
	ExprNodeList *guards;  /**< The expressions to evaluate. */
	BlockNodeList *blocks; /**< The blocks of code to execute. */
	BlockNode *def;        /**< An optional default block of code. */
} SwitchStmtNode;

/**
 * Stores a return statement.  This statement signals that control should be
 * returned to the caller with a status value.
 */
typedef struct {
	ExprNode *value; /**< The value to return. */
} ReturnStmtNode;

/**
 * Stores a loop statement.  This statement repeatedly executes its \a body
 * while \a guard evaluates to true, executing \a update at the end of each
 * cycle.
 */
typedef struct {
	IdentifierNode *name; /**< The name of the loop. */
	IdentifierNode *var;  /**< The variable to be updated. */
	ExprNode *guard;      /**< The expression to determine continuation. */
	ExprNode *update;     /**< The expression to update \a var with. */
	BlockNode *body;      /**< The code to execute at each iteration. */
} LoopStmtNode;

/**
 * Stores a deallocation statement.  This statement releases the resources used
 * by a variable.
 */
typedef struct {
	IdentifierNode *target; /**< The variable to deallocate. */
} DeallocationStmtNode;

/**
 * Stores a cast expression.  This expression evaluates an expression and casts
 * its value to a particular type.
 */
typedef struct {
	ExprNode *target;  /**< The expression to cast. */
	TypeNode *newtype; /**< The type to cast \a target to. */
} CastExprNode;

/**
 * Stores a function call expression.  This expression calls a named function
 * and evaluates to the return value of that function.
 */
typedef struct {
	IdentifierNode *scope; /**< The scope to call the function in. */
	IdentifierNode *name;  /**< The name of the function to call. */
	ExprNodeList *args;    /**< The arguments to supply the function. */
} FuncCallExprNode;

/**
 * Represents the type of operation an OpExprNode performs.
 */
typedef enum {
	OP_ADD,  /**< Addition. */
	OP_SUB,  /**< Subtraction. */
	OP_MULT, /**< Multiplication. */
	OP_DIV,  /**< Division. */
	OP_MOD,  /**< Modulo. */
	OP_MAX,  /**< Maximum. */
	OP_MIN,  /**< Minimum. */

	OP_AND,  /**< Logical AND. */
	OP_OR,   /**< Logical OR. */
	OP_XOR,  /**< Logical XOR. */
	OP_NOT,  /**< Logical NOT. */

	OP_EQ,   /**< Equality. */
	OP_NEQ,  /**< Inequality. */

	OP_CAT   /**< String concatenation. */
} OpType;

/**
 * Stores an operation expression.  This expression applies an operator to its
 * arguments.
 */
typedef struct  {
	OpType type;        /**< The type of operation to perform. */
	ExprNodeList *args; /**< The arguments to perform the operation on. */
} OpExprNode;

/**
 * \name MainNode modifiers
 *
 * Functions for creating and deleting MainNodes.
 */
/**@{*/
MainNode *createMainNode(BlockNode *);
void deleteMainNode(MainNode *);
/**@}*/

/**
 * \name BlockNode modifiers
 *
 * Functions for creating and deleting single or multiple BlockNodes.
 */
/**@{*/
BlockNode *createBlockNode(StmtNodeList *);
void deleteBlockNode(BlockNode *);
BlockNodeList *createBlockNodeList(void);
int addBlockNode(BlockNodeList *, BlockNode *);
void deleteBlockNodeList(BlockNodeList *);
/**@}*/

/**
 * \name IdentifierNode modifiers
 *
 * Functions for creating and deleting single or multiple IdentifierNodes.
 */
/**@{*/
IdentifierNode *createIdentifierNode(IdentifierType, void *, IdentifierNode *, const char *, unsigned int);
void deleteIdentifierNode(IdentifierNode *);
IdentifierNodeList *createIdentifierNodeList(void);
int addIdentifierNode(IdentifierNodeList *, IdentifierNode *);
void deleteIdentifierNodeList(IdentifierNodeList *);

/**@}*/

/**
 * \name TypeNode modifiers
 *
 * Functions for creating and deleting TypeNodes.
 */
/**@{*/
TypeNode *createTypeNode(ConstantType);
void deleteTypeNode(TypeNode *);
/**@}*/

/**
 * \name StmtNode modifiers
 *
 * Functions for creating and deleting single or multiple of StmtNodes.
 */
/**@{*/
StmtNode *createStmtNode(StmtType, void *);
void deleteStmtNode(StmtNode *);
StmtNodeList *createStmtNodeList(void);
int addStmtNode(StmtNodeList *, StmtNode *);
void deleteStmtNodeList(StmtNodeList *);
/**@}*/

/**
 * \name CastStmtNode modifiers
 *
 * Functions for creating and deleting CastStmtNodes.
 */
/**@{*/
CastStmtNode *createCastStmtNode(IdentifierNode *, TypeNode *);
void deleteCastStmtNode(CastStmtNode *);
/**@}*/

/**
 * \name PrintStmtNode modifiers
 *
 * Functions for creating and deleting PrintStmtNodes.
 */
/**@{*/
PrintStmtNode *createPrintStmtNode(ExprNodeList *, int);
void deletePrintStmtNode(PrintStmtNode *);
/**@}*/

/**
 * \name InputStmtNode modifiers
 *
 * Functions for creating and deleting InputStmtNodes.
 */
/**@{*/
InputStmtNode *createInputStmtNode(IdentifierNode *);
void deleteInputStmtNode(InputStmtNode *);
/**@}*/

/**
 * \name AssignmentStmtNode modifiers
 *
 * Functions for creating and deleting AssignmentStmtNodes.
 */
/**@{*/
AssignmentStmtNode *createAssignmentStmtNode(IdentifierNode *, ExprNode *);
void deleteAssignmentStmtNode(AssignmentStmtNode *);
/**@}*/

/**
 * \name DeclarationStmtNode modifiers
 *
 * Functions for creating and deleting DeclarationStmtNodes.
 */
/**@{*/
DeclarationStmtNode *createDeclarationStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *, TypeNode *, IdentifierNode *);
void deleteDeclarationStmtNode(DeclarationStmtNode *);
/**@}*/

/**
 * \name IfThenElseStmtNode modifiers
 *
 * Functions for creating and deleting IfThenElseStmtNodes.
 */
/**@{*/
IfThenElseStmtNode *createIfThenElseStmtNode(BlockNode *, BlockNode *, ExprNodeList *, BlockNodeList *);
void deleteIfThenElseStmtNode(IfThenElseStmtNode *);
/**@}*/

/**
 * \name SwitchStmtNode modifiers
 *
 * Functions for creating and deleting SwitchStmtNodes.
 */
/**@{*/
SwitchStmtNode *createSwitchStmtNode(ExprNodeList *, BlockNodeList *, BlockNode *);
void deleteSwitchStmtNode(SwitchStmtNode *);
/**@}*/

/**
 * \name ReturnStmtNode modifiers
 *
 * Functions for creating and deleting ReturnStmtNodes.
 */
/**@{*/
ReturnStmtNode *createReturnStmtNode(ExprNode *);
void deleteReturnStmtNode(ReturnStmtNode *);
/**@}*/

/**
 * \name LoopStmtNode modifiers
 *
 * Functions for creating and deleting LoopStmtNodes.
 */
/**@{*/
LoopStmtNode *createLoopStmtNode(IdentifierNode *, IdentifierNode *, ExprNode *, ExprNode *, BlockNode *);
void deleteLoopStmtNode(LoopStmtNode *);
/**@}*/

/**
 * \name DeallocationStmtNode modifiers
 *
 * Functions for creating and deleting DeallocationStmtNodes.
 */
/**@{*/
DeallocationStmtNode *createDeallocationStmtNode(IdentifierNode *);
void deleteDeallocationStmtNode(DeallocationStmtNode *);
/**@}*/

/**
 * \name FuncDefStmtNode modifiers
 *
 * Functions for creating and deleting FuncDefStmtNodes.
 */
/**@{*/
FuncDefStmtNode *createFuncDefStmtNode(IdentifierNode *, IdentifierNode *, IdentifierNodeList *, BlockNode *);
void deleteFuncDefStmtNode(FuncDefStmtNode *);
/**@}*/

/**
 * \name AltArrayDefStmtNode modifiers
 *
 * Functions for creating and deleting AltArrayDefStmtNodes.
 */
/**@{*/
AltArrayDefStmtNode *createAltArrayDefStmtNode(IdentifierNode *, BlockNode *, IdentifierNode *);
void deleteAltArrayDefStmtNode(AltArrayDefStmtNode *);
/**@}*/

/**
 * \name ExprNode modifiers
 *
 * Functions for creating and deleting single or multiple ExprNodes.
 */
/**@{*/
ExprNode *createExprNode(ExprType, void *);
void deleteExprNode(ExprNode *);
ExprNodeList *createExprNodeList(void);
int addExprNode(ExprNodeList *, ExprNode *);
void deleteExprNodeList(ExprNodeList *);
/**@}*/

/**
 * \name CastExprNode modifiers
 *
 * Functions for creating and deleting CastExprNodes.
 */
/**@{*/
CastExprNode *createCastExprNode(ExprNode *, TypeNode *);
void deleteCastExprNode(CastExprNode *);
/**@}*/

/**
 * \name FuncCallExprNode modifiers
 *
 * Functions for creating and deleting FuncCallExprNodes.
 */
/**@{*/
FuncCallExprNode *createFuncCallExprNode(IdentifierNode *, IdentifierNode *, ExprNodeList *);
void deleteFuncCallExprNode(FuncCallExprNode *);
/**@}*/

/**
 * \name OpExprNode modifiers
 *
 * Functions for creating and deleting OpExprNodes.
 */
/**@{*/
OpExprNode *createOpExprNode(OpType, ExprNodeList *);
void deleteOpExprNode(OpExprNode *);
/**@}*/

/**
 * \name Utilities
 *
 * Functions for performing helper tasks.
 */
/**@{*/
int acceptToken(Token ***, TokenType);
int peekToken(Token ***, TokenType);
int nextToken(Token ***, TokenType);
/**@}*/

/**
 * \name Parsing functions
 *
 * Functions for parsing a stream of tokens.
 */
/**@{*/
ConstantNode *parseConstantNode(Token ***);
TypeNode *parseTypeNode(Token ***);
IdentifierNode *parseIdentifierNode(Token ***);
ExprNode *parseExprNode(Token ***);
StmtNode *parseStmtNode(Token ***);
BlockNode *parseBlockNode(Token ***);
MainNode *parseMainNode(Token **);
ExprNode *parseCastExprNode(Token ***);
ExprNode *parseConstantExprNode(Token ***);
ExprNode *parseIdentifierExprNode(Token ***);
ExprNode *parseFuncCallExprNode(Token ***);
ExprNode *parseOpExprNode(Token ***);
StmtNode *parseCastStmtNode(Token ***);
StmtNode *parsePrintStmtNode(Token ***);
StmtNode *parseInputStmtNode(Token ***);
StmtNode *parseAssignmentStmtNode(Token ***);
StmtNode *parseDeclarationStmtNode(Token ***);
StmtNode *parseIfThenElseStmtNode(Token ***);
StmtNode *parseSwitchStmtNode(Token ***);
StmtNode *parseBreakStmtNode(Token ***);
StmtNode *parseReturnStmtNode(Token ***);
StmtNode *parseLoopStmtNode(Token ***);
StmtNode *parseDeallocationStmtNode(Token ***);
StmtNode *parseFuncDefStmtNode(Token ***);
StmtNode *parseAltArrayDefStmtNode(Token ***);
/**@}*/

/**
 * \name ConstantNode modifiers
 *
 * Functions for creating and deleting ConstantNode.
 */
/**@{*/
ConstantNode *createBooleanConstantNode(int);
ConstantNode *createIntegerConstantNode(long long int);
ConstantNode *createFloatConstantNode(float);
ConstantNode *createStringConstantNode(char *);
void deleteConstantNode(ConstantNode *);
/**@}*/

#endif /* __PARSER_H__ */
