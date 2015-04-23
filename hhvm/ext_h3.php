<?hh

define('H3_NOTHING', null);
define('H3_FLOATY', 2.71);

function h3_hello_world() {
  echo "Hello World!\n";
}

<<__Native>>
function h3_add(int $a, int $b): int;

<<__Native>>
function h3_enum_greetings(): array;

<<__Native>>
function h3_iterate(array $arr): void;
