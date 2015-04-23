<?hh

$m = new MyClass;
var_dump($m->foo(123));
var_dump($m->foo(-123));

var_dump(MyClass::numeric("456"));
var_dump(MyClass::numeric("4.56"));
var_dump(MyClass::numeric("banana"));
