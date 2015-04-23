<?hh

define('H3_NOTHING', null);
define('H3_FLOATY', 2.71);

<<__Native>>
function h3_bzlib_version(): string;

function h3_hello_world() {
  echo "Hello World!\n";
}

<<__Native>>
function h3_add(int $a, int $b): int;

<<__Native>>
function h3_enum_greetings(): array;

<<__Native>>
function h3_iterate(array $arr): void;

<<__NativeData("MyClass")>>
class MyClass {
  <<__Native>>
  public function foo(int $bar): ?string;

  <<__Native>>
  public static function numeric(string $val): mixed;

  <<__Native>>
  public function get(): int;

  <<__Native>>
  public function inc(): int;

  <<__Native>>
  public function dec(): int;
}
