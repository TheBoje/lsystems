#ifndef LOCATION_H
#define LOCATION_H

typedef struct yyltype {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} yyltype;

#define YYLTYPE yyltype

#endif // LOCATION_H
