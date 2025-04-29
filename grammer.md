```
query          → selectStmt | insertStmt | createStmt | updateStmt | deleteStmt ;
updateStmt     → "UPDATE" table "SET" column "=" value ( "," column "=" value)* ( "WHERE" comparison )? ";" ;
deleteStmt     → "DELETE" table "FROM" table ( "WHERE" comparison )? ";" ;
createStmt     → "CREATE" "TABLE" table "(" column  type ("," column type)* ")" ";" ;
selectStmt     → "SELECT" ( "*" | column ("," column)* ) "FROM" table ( "WHERE" comparison )?  ";" ;
insertStmt     → "INSERT" "INTO" table ( "(" column ("," column)* ") )?"
                 "VALUES" "(" value ("," value)* ")" ";" ;
comparison     → ( column | value ) ( ">" | ">=" | "<" | "<=" | "!=" | "=" ) ( column | value ) ;
column         → IDENTIFIER
table          → IDENTIFIER
value          → NUMBER | STRING | BOOLEAN
type           → INT | STRING
```
