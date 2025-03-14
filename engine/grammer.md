```
query          → statement EOF ;
statement      → selectStmt | insertStmt ;
selectStmt     → "SELECT" ( "*" | column ("," column)* ) "FROM" table ";" ;
insertStmt     → "INSERT" "INTO" table ( "(" column ("," column)* ") )?"
                 "VALUES" "(" value ("," value)* ")" ";" ;
column         → IDENTIFIER
table          → IDENTIFIER
value          → NUMBER | STRING
```
