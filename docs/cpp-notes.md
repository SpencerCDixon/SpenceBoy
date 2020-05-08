# C++ Learning Notes

## CMake

* [Modern CMake](https://cliutils.gitlab.io/modern-cmake/chapters/testing.html)

## Andreas Questions

### Kind of a general C/C++ question: why are a lot of your std lib .h files and not .h + .cpp? How do I know when I should be putting all functionality in .h and not create a .cpp as well?

Generally speaking, I put template classes in header files, since it's required. If you declare a template<typename T> Vector<T> for example, no tangible class exists yet. It's only when you actually say something like Vector<int> later on that the compiler instantiates a copy of the Vector template with int in place of T. Since the compiler cannot "see" into Vector.cpp when compiling some other cpp file, it would not be possible to have Vector functions in the cpp file.

For non-templated code, the main reason to have definitions (and not just declarations) in header files is to make sure that it gets inlined (for performance reasons.) Trivial code like Vector::operator[] can be streamlined to very little generated code at the call site if inlined, but if it's moved into a cpp file, all callers will have no choice but to make a function call, pushing arguments on the stack, etc.

The main downside of putting code in headers is compile time. Since headers are reparsed and recompiled in every cpp file that includes them (the C and C++ preprocessors is basically just a copy-paste machine), many large headers means a lot of code to compile.

As with everything, I recommend messing with it and trying out different things to see how you like to organize your own code. And I don't mean to hand-wave about "performance reasons" btw. When you get things up and running I can show you some ways (on both macOS and Linux) to systematically work out which parts are running slow and how to fix them.

## Tips

Use the `-Wimplicit-fallthrough` compiler flag to prevent missing `break`'s in switch statements which can cause a lot of pain and suffering.