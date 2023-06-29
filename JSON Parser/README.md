# JSON Parser
Project written as college assignment. It purpose was to create working JSON parser and interface to get JSON objects (JPath). I got max points for it, but I doubt that it is error prone. It
wasn't strict tested and might contain bugs

# Input
## Commands
```
h - show help
o [file path] - opens file
c - checks if loaded file is proper JSON file
p [JSONPath] - gets and shows data at given path
z [JSONPath] = [data] - gets and changes data at given path
cls - clears the screen
q - quits the program
```
## JPath
```
$["map-object"][number]
"" - gets map key
number - gets object from array
If parent objects are not valid type, exception is thrown (like Key cannot be used in array)
```
# Implementation
Standard Token implementation is used. There are various parsers: **JSON Parser**, **JPath Parser**, **JSON String Parser** (The last one had to be written, cause i had some troubles using standard JSON parser
in JPATH objects)

# Further Development
Further development is not planned. There may still exists small bugs, pretty sure JPath had few. If anyone in the future would like to use this I can track and get rid of some of them. Still this is
great foundation for further development if needed anytime
