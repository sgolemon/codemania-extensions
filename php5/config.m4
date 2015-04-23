dnl $Id$
dnl config.m4 for extension p5

PHP_ARG_ENABLE(p5, whether to enable p5 support,
[  --disable-p5            Disable p5 support], yes)

if test "$PHP_P5" != "no"; then
  PHP_NEW_EXTENSION(p5, php_p5.c, $ext_shared)
fi
