# C++ Learning Notes

## CMake

* [Modern CMake](https://cliutils.gitlab.io/modern-cmake/chapters/testing.html)
* [Preventing Library Warnings](https://foonathan.net/2018/10/cmake-warnings/)

## Andreas Questions

### Kind of a general C/C++ question: why are a lot of your std lib .h files and not .h + .cpp? How do I know when I should be putting all functionality in .h and not create a .cpp as well?

Generally speaking, I put template classes in header files, since it's required. If you declare a template<typename T> Vector<T> for example, no tangible class exists yet. It's only when you actually say something like Vector<int> later on that the compiler instantiates a copy of the Vector template with int in place of T. Since the compiler cannot "see" into Vector.cpp when compiling some other cpp file, it would not be possible to have Vector functions in the cpp file.

For non-templated code, the main reason to have definitions (and not just declarations) in header files is to make sure that it gets inlined (for performance reasons.) Trivial code like Vector::operator[] can be streamlined to very little generated code at the call site if inlined, but if it's moved into a cpp file, all callers will have no choice but to make a function call, pushing arguments on the stack, etc.

The main downside of putting code in headers is compile time. Since headers are reparsed and recompiled in every cpp file that includes them (the C and C++ preprocessors is basically just a copy-paste machine), many large headers means a lot of code to compile.

As with everything, I recommend messing with it and trying out different things to see how you like to organize your own code. And I don't mean to hand-wave about "performance reasons" btw. When you get things up and running I can show you some ways (on both macOS and Linux) to systematically work out which parts are running slow and how to fix them.

## Tips

Use the `-Wimplicit-fallthrough` compiler flag to prevent missing `break`'s in switch statements which can cause a lot of pain and suffering.

In general, only put things in header files that need to be exposed to other classes/files. Encapsulate
as much as possible into .cpp files.

## Const

This says that showName returns a constant int value -- one that is immutable. 
Of course since the int is returned by value the presence of const here does not play any role.

```c++
const int MyClass::showName(string id)
```

And this says that showName does not modify the observable state of MyClass 
(technically: it does not modify any member that is not declared mutable), 
and therefore you are allowed to call it on a value of type const MyClass.

```c++
int MyClass::showName(string id) const
```

## Constructor Init Lists

Question: When to use these initializer lists vs initializing things in the constructor?

Answer: Always preferable to use the init lists. That way members will never be in an in-between state. Some things
MUST be inited (like ref members, which must always be assigned to something and can't be rebound).

## Third Party Headers

Sometimes headers can get out of control with #ifdefs to manage certain platform or compiler specific functionality. 
For example:

```c++
#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#    include <SDL.h>
#    pragma clang diagnostic pop
#else
#    include <SDL.h>
#endif
```

A useful tip for handling these is to create a single project specific header (ie. `MyProjectSDL.h`) which
can contain all this sillyness and other call sites can import that clean looking header.

## Disk IO

Files in `/tmp` will be significantly faster than inside other directories.
These are backed by RAM so it won't do disk I/O thus speeding things up a LOT.

## References vs. Pointers

When trying to decide between these two one thing to think about is nullability. If an entity can be
null than by definition it cannot be a reference since references are always non-null. When something
can start as a `nullptr` and then be defined look to use some sort of smart pointer or raw pointer.

When returning a reference from a pointer you must first dereference the pointer in order to return
a reference to the actual value and not the pointer. For example:

```c++
class Widget {
public:
    MyThing& thing() { return *m_my_thing; }    // Needs to be dereferenced to then return a ref
    OtherItem& item() { return m_other_item; }  // Already a real item and not a pointer so can return directly
private:
    MyThing* m_my_thing;
    OtherItem m_other_item;
};
```

## Operator Overloading

In Rust, there is a concept of `Into<T>` for easily transitioning between types. In C++,
we can use operator overloading to achieve similar results. Here is an example of me using
operator overloading to convert between my custom `Rect` type and the SDL rect type:

```c++
struct Rect {
    int x;
    int y;
    int width;
    int height;
    
    operator SDL_Rect() const {
        return SDL_Rect { this->x, this->y, this->width, this->height };
    }
};
```