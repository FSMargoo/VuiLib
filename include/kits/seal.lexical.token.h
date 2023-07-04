#pragma once

/////////////////////////////////////////////////////////////////////////////
//         S E A L  P R O G R A M M I N G  L A N G U A G E (seal-lang.org)
//
//  seal.lexical.token.h :
//       @description :  This header folder defines the token required by
//       leixcal
//       @birth       :  2021/2.15
//       @operator    :  Margoo
//

///////////////////////////////////////////////////////////////
//   _leixcal_token[typedef with seal_token] :
//          @description :   Enumerate tokens required for seal lexical analysis
//          @birth       :   2021/2.15
//          @operator    :   Margoo
typedef enum _lexical_token
{
	NO_STATUS_TOKEN = (int)0,

	UNKNOW_TOKEN,

	// Quantitative statement
	CONST_NUMBER,
	CONST_STRING,
	CONST_BOOLEAN,

	// Keyword token
	INT_TOKEN,
	BOOL_TOKEN,
	STRING_TOKEN,
	CHAR_TOKEN,
	FLOAT_TOKEN,
	DEF_TOKEN,
	IF_TOKEN,
	ELSE_TOKEN,
	ELIF_TOKEN,
	SWITCH_TOKEN,
	CASE_TOKEN,
	DEFAULT_TOKEN,
	WHILE_TOKEN,
	CONTINUE_TOKEN,
	BREAK_TOKEN,
	OPERATOR_TOKEN,
	CLASS_TOKEN,
	PUBLIC_TOKEN,
	PRIVATE_TOKEN,
	RETURN_TOKEN,
	ENUM_TOKEN,
	USING_TOKEN,
	FROM_TOKEN,
	NULL_TOKEN,
	SAFENULL_TOKEN,
	DELETE_TOKEN,
	DO_TOKEN,
	FOR_TOKEN,
	PARAD_TOKEN,
	STATIC_TOKEN,
	NEW_STATIC_TOKEN,
	UN_GC_TOKEN,
	FORCE_DELETE_TOKEN,
	NO_REFER_COUNT_TOKEN,
	NO_OLD_TOKEN,
	CONST_TOKEN,
	// Dynamic library library implementation keywords
	SEAL_LIBRARY_TYPE_TOKEN,
	SEAL_LIBRARY_FUNCTION_TOKEN,
	SEAL_LIBRARY_RUN_FUNCTION_TOKEN,

	TYPE_ALIAS_TOKEN,
	TYPE_DEFINE_TOKEN,
	FUNCTION_TOKEN,
	RETR_FOR_TOKEN,

	NAMESPACE_TOKEN,

	COMMENT_TOKEN,

	LEFT_BRACKETS /*(*/,
	RIGHT_BRACKETS,		   /*)*/
	MIDDLE_LEFT_BRACKETS /* [ */,
	MIDDLE_RIGHT_BRACKETS, /* ] */
	BIG_LEFT_BRACKETS /* { */,
	BIG_RIGHT_BRACKETS,	   /* } */

	UNKNOW_SMYBOL,

	SEMICOLON_TOKEN /* ; */,
	DOT_TOKEN /* . */,
	COMMA_TOKEN /* , */,
	COLON_TOKEN /* : */,
	PLUS_SIGN_TOKEN /* + */,
	MINUS_SIGN_TOKEN /* - */,
	MULTIPLICATION_SIGN_TOKEN /* * */,
	SLASH_TOKEN /* / */,
	MODULO_SIGN_TOKEN /* % */,
	DOUBLE_PLUS_TOKEN /* ++ */,
	UNDERSCORE_TOKEN /* _ */,
	DOUBLE_MINUS_TOKEN /* -- */,
	EQUAL_SIGN_TOKEN /* = */,
	DOUBLE_EQUAL_SIGN_TOKEN /* == */,
	EXCLAMATION_MARK /* ! */,
	NOT_EQUAL_TOKEN /* != */,
	MORE_THAN_TOKEN /* > */,
	LESS_THAN_TOKEN /* < */,
	MORE_THAN_EQUAL_TOKEN /* >= */,
	LESS_THAN_EQUAL_TOKEN /* <= */,
	VERSUS_TOKEN /* & */,
	LOGICALAND_TOKEN /* && */,
	VERTICAL_BAR_TOKEN /* | */,
	LOGICAL_OR_TOKEN /* || */,
	CARET_TOKEN /* ^ */,
	TILDE_TOKEN /* ~ */,
	BR_LEFT_SHIFT_OPERATOR_TOKEN /* << */,
	BR_RIGHT_SHIFT_OPERATOR_TOKEN /* >> */,
	ADDITIONAND_TOKEN /* += */,
	MINUSITIONAND_TOKEN /* -= */,
	MULTIPLYAND_TOKEN /* *= */,
	EXCEPTAND_TOKEN, /* /= */
	BITWISEAND_TOKEN /* &= */,
	INHERITANCE_TOKEN /* <- */,
	BITWISEANDDIFFERENT_TOKEN /* ^= */,
	BITWISE_OR_TOKEN, /* |= */

	EOF_TOKEN
} seal_token;