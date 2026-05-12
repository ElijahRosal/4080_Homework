# Question 2

I added computed field access with bracket syntax: `instance[expression]` and `instance[expression] = value`. The expression inside the brackets evaluates to the field name at runtime, so code can build the name imperatively and still use it for lookup or assignment.

I chose brackets because they are already the standard spelling for computed lookup in many dynamically typed languages, and they fit naturally beside Lox's existing dot syntax for direct field access.
