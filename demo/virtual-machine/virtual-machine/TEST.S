dat     string      $welcome    "Test"
dat     qword       $test1      1
dat     qword       $test2      2

mov     ebx      80
mov     $test2   ebx
mova    $test2   $test1