
/*

+	Regular Expression Engine for C language 1.2.0
+	REEC 1.2.0
+	(C)TOK WANG QUANWEI

+	Regular Engine is a free software created by (c) Techniques of Knowledge since 20090528,	
+	you can redistribute it and/or modifyit under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope	that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
+																													
+	(c) 	Techniques of Knowledge 
+		an open source group since 2008	
+		page : http://www.tok.cc
+		email : wqw85@sina.com
+
*/

# include "malloc.h"

void* reec_malloc ( int length ) {

	//	author : WANG QUANWEI
	//	since : 20100419
	//	(C)TOK

	return malloc ( length ) ;
	
}

void recc_free ( void* buffer ) {

	//	author : WANG QUANWEI
	//	since : 20100419
	//	(C)TOK

	if ( !buffer ) return ;
	
	free ( buffer ) ;
	
}

