# Question 3

I chose a JavaScript-style delete statement for removing instance fields. It returns a boolean, so user code can tell whether a field was actually removed, and the field is removed from the instance's hash table rather than just being overwritten with nil.

I also allowed computed field names with bracket syntax so deletion can target either direct field names or names assembled at runtime.
