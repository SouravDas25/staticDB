#ifndef TYPECODE_H_INCLUDED
#define TYPECODE_H_INCLUDED

/** @brief ads type_code holder enum.

 */

 #define ADS_MAXTYPE 32

typedef enum _ads_typeCode {
VOID_TC=-1000,
CHAR_TC,
INT_TC,
INT32_TC,
INT64_TC,
UINT_TC,
BOOL_TC,
HEX_TC,
OCT_TC,
FLOAT_TC,
DOUBLE_TC,
STRING_TC,
QSTRING_TC,
ZBUFFER_TC } ads_typeCode;


#endif // TYPECODE_H_INCLUDED
