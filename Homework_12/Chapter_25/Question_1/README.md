# Homework 12, Chapter 25, Question 1

Change clox so it only wraps functions in ObjClosures when a function actually closes over upvalues.

## Answer

The selective-wrapping design is a good trade-off when you care about the common case. Functions with no upvalues avoid an extra allocation and one layer of indirection, so calls to ordinary functions are cheaper. The downside is that the implementation becomes more complicated because the VM has to distinguish between plain ObjFunction values and closures, and the call path needs an extra branch to handle both cases.

Always wrapping every function in a closure is simpler. It gives the runtime one uniform callable representation, which makes the VM and compiler easier to reason about. The cost is that every function call pays for closure allocation and closure dispatch, even when no captured variables exist.

In practice, the performance decision should be based on the workload mix, not just one benchmark. Programs that never use closures should matter because they represent a common and important case, and the selective version should clearly win there. Programs that use closures heavily also matter because they show the overhead of the extra branching and wrapper management. If one benchmark gets faster and another gets slower, I would weight them by how often each pattern appears in the intended use of the language. The best choice is usually the one that improves the common path without making closure-heavy code significantly worse.

My conclusion is that selective wrapping is the better implementation strategy if the goal is to optimize normal function calls while keeping closure support correct. It is more complex than always wrapping, but the extra complexity is localized and the performance gain applies to the most frequent case.
