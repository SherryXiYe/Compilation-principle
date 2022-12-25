
;; Function myplus (_Z6myplusii, funcdef_no=1522, decl_uid=36328, cgraph_uid=436, symbol_order=439)

myplus (int x, int y)
{
  int D.39988;

  D.39988 = x + y;
  goto <D.39989>;
  <D.39989>:
  return D.39988;
}



;; Function main (main, funcdef_no=1523, decl_uid=36330, cgraph_uid=437, symbol_order=440)

main ()
{
  struct basic_ostream & D.39996;
  struct basic_ostream & D.39995;
  struct basic_ostream & D.39994;
  const int add;
  int t;
  int i;
  int b;
  int a;
  int D.39992;

  a = 0;
  b = 1;
  i = 1;
  add = 1;
  std::basic_istream<char>::operator>> (&cin, &globe_n);
  D.39994 = std::basic_ostream<char>::operator<< (&cout, a);
  _1 = D.39994;
  std::basic_ostream<char>::operator<< (_1, endl);
  D.39995 = std::basic_ostream<char>::operator<< (&cout, b);
  _2 = D.39995;
  std::basic_ostream<char>::operator<< (_2, endl);
  <D.39990>:
  globe_n.0_3 = globe_n;
  if (i >= globe_n.0_3) goto <D.36383>; else goto <D.39991>;
  <D.39991>:
  t = b;
  b = myplus (a, b);
  D.39996 = std::basic_ostream<char>::operator<< (&cout, b);
  _4 = D.39996;
  std::basic_ostream<char>::operator<< (_4, endl);
  a = t;
  i = i + 1;
  goto <D.39990>;
  <D.36383>:
  D.39992 = 0;
  goto <D.39993>;
  D.39992 = 0;
  goto <D.39993>;
  <D.39993>:
  return D.39992;
}



;; Function __static_initialization_and_destruction_0 (_Z41__static_initialization_and_destruction_0ii, funcdef_no=2009, decl_uid=39980, cgraph_uid=923, symbol_order=950)

__static_initialization_and_destruction_0 (int __initialize_p, int __priority)
{
  if (__initialize_p == 1) goto <D.39997>; else goto <D.39998>;
  <D.39997>:
  if (__priority == 65535) goto <D.39999>; else goto <D.40000>;
  <D.39999>:
  std::ios_base::Init::Init (&__ioinit);
  __cxa_atexit (__dt_comp , &__ioinit, &__dso_handle);
  goto <D.40001>;
  <D.40000>:
  <D.40001>:
  goto <D.40002>;
  <D.39998>:
  <D.40002>:
  return;
}



;; Function _GLOBAL__sub_I_globe_n (_GLOBAL__sub_I_globe_n, funcdef_no=2010, decl_uid=39986, cgraph_uid=924, symbol_order=1080)

_GLOBAL__sub_I_globe_n ()
{
  __static_initialization_and_destruction_0 (1, 65535);
  return;
}


