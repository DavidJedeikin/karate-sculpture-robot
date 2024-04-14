# Software Improvements

## 1. Add a hardware abstraction layer (HAL)
Add a HAL layer, enabling the abstraction of all hardware specific implementations. 

There is currently no hardware abstration - all the hardware components in `src/hardware` configure the hardware peripherals directly. This is bad for a number of reasons: 

1. Complicates/prevents unit testing 
2. Prevents portability between compute platforms 
3. Prevents hardware emulation (We are unable to implement emulated versions of the hardware components) 
4. Hardware configuration is deeply coupled with the application implementation 

An example of an abstract class for a digital input would look something like 

```cpp
class IDigitalInput {
public: 
    enum class State {
        low, 
        high
    };

    virtual ~IDigitalInput() {} 
    virtual State getState() const = 0;
};
``` 

This includes abstracting all time related components such as Arduino's `delay` method. To do so, two new components will be needed: a `testTimer` and a `timerService`. The `testTimer` will be a test-specifc timer tool with test trigger methods such a `fireTimer` to enable determining testing of time-dependent components. The `timerService` will be injected into time-dependent components and will generate either _real_ hardware-dependent application timers or `testTimers` depending on the context (application or testing). 

## 2. Where appropriate, refactor classes to use dependency injection 
Ensure all classes that use HAL or other interface components only accept these components via dependency inject. Avoid composition as this complicates testing.  

## 3. Add unit tests 
1. Write [gMock](https://google.github.io/googletest/gmock_for_dummies.html) classes for all interface classes
2. Write unit tests using [gTest](https://google.github.io/googletest/gmock_cook_book.html) & gMock

## 4. Add CI pipeline 

1. Stage one: Build :white_check_mark: Software builds succesfully
2. State two: Static analysis :white_check_mark: Software passes linting & formatting (if not, format software)
3. State three: Unit tests :white_check_mark: All unit tests passed
4. State four (maybe later) : Integration tests :white_check_mark: All integration tests pass 

## 5. Template the `LinearMap` class 
This LinearMap class is currently hard coded to use `floats`, however, it might be handly to map two ranges of other types (`int`, `uintx_t`, etc...)

## 6. Improve the `BodyMotion::SingleDanceMotion` method
This method is hardware specific and assumes the waist joint range is less than the arm joint range. This method also has very little assumption/edge-case checking. 
