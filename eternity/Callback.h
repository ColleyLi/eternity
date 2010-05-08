#ifndef CALLBACK_H
#define CALLBACK_H

#include "boost/function.hpp"


//!! delay needs to be worked out
// of this class and into the window->addCallback() code instead.


// base class.  oh the trickery.
struct Callback
{
  double when ;

  /// A beautiful PURELY
  /// virtual function, necessitating
  /// every derived class to 
  /// implement this function
  virtual void exec() = 0 ;
} ;

// Callback function that takes 
// 0 arguments.
struct Callback0 : public Callback
{
  boost::function<void ()> func ;

  Callback0(){}

  Callback0( double delay, void (*theFunc)() )
  {
    when = delay ;
    func = theFunc ;
  }

  void exec()
  {
    func() ;
  }
} ;

// Now for generic funcs
// that take 1-3 args.
// Using template metaprogramming.
// Fun times.
// Don't try to understand this
// thy head hurteth thee.
template <typename typeArg1>
struct Callback1 : public Callback
{
  boost::function<void (typeArg1)> func ;
  typeArg1 argument1 ;

  Callback1(){}

  Callback1( double delay, void (*theFunc)( typeArg1 theArg1 ), typeArg1 arg1 )
  {
    when = delay ;
    func = theFunc ;
    argument1 = arg1 ;
  }

  void exec()
  {
    func( argument1 ) ;
  }
} ;

// LOOK AWAY!!  Oh the monstrosity..
// apparently "variadic templates"
// would clean this up, but hey, it works for now
template <typename typeArg1, typename typeArg2>
struct Callback2 : public Callback
{
  boost::function<void (typeArg1, typeArg2)> func ;
  typeArg1 argument1 ;
  typeArg2 argument2 ;
  
  Callback2(){}

  Callback2( double delay,
            void (*theFunc)(
              typeArg1 theArg1,
              typeArg2 theArg2
            ),
            typeArg1 arg1,
            typeArg2 arg2 )
  {
    when = delay ;
    func = theFunc ;
    argument1 = arg1 ;
    argument2 = arg2 ;
  }

  void exec()
  {
    func( argument1, argument2 ) ;
  }
} ;


// really it just gets worse from here,
// i suggest you go back
template <typename typeArg1,
          typename typeArg2,
          typename typeArg3>
struct Callback3 : public Callback
{
  boost::function<void (typeArg1, typeArg2, typeArg3)> func ;
  typeArg1 argument1 ;
  typeArg2 argument2 ;
  typeArg3 argument3 ;

  Callback3(){}

  Callback3( double delay,
            void (*theFunc)(
              typeArg1 theArg1,
              typeArg2 theArg2,
              typeArg3 theArg3
            ),
            typeArg1 arg1,
            typeArg2 arg2,
            typeArg3 arg3 )
  {
    when = delay ;
    func = theFunc ;
    argument1 = arg1 ;
    argument2 = arg2 ;
    argument3 = arg3 ;
  }

  void exec()
  {
    func( argument1, argument2, argument3 ) ;
  }
} ;


// oh the shameless ..
template <typename typeArg1,
          typename typeArg2,
          typename typeArg3,
          typename typeArg4>
struct Callback4 : public Callback
{
  boost::function<void (typeArg1, typeArg2, typeArg3, typeArg4)> func ;
  typeArg1 argument1 ;
  typeArg2 argument2 ;
  typeArg3 argument3 ;
  typeArg4 argument4 ;

  Callback4(){}

  Callback4( double delay,
             void (*theFunc)(
               typeArg1 theArg1,
               typeArg2 theArg2,
               typeArg3 theArg3,
               typeArg4 theArg4
             ),
             typeArg1 arg1,
             typeArg2 arg2,
             typeArg3 arg3,
             typeArg4 arg4 )
  {
    when = delay ;
    func = theFunc ;
    argument1 = arg1 ;
    argument2 = arg2 ;
    argument3 = arg3 ;
    argument4 = arg4 ;
  }

  void exec()
  {
    func( argument1, argument2, argument3, argument4 ) ;
  }
} ;

// WELL!  I guess if you want more than 4+ arguments
// you can code it yourself Mr.! (OR Ms.)
// Just follow the pattern above!

// programming is an art..

// no but honestly, I'm not really happy with this
// solution, but it works.  it could possibly
// be improved.  but again, it works now
// with reasonably clean syntax.





#endif