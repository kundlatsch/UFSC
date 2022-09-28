 .global strlen
      

strlen:

 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 movl 8(%ebp),%edx
 movl %edx,%edi
 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

L5:
 subl $4,%edi


 .p2align 4,,7
L10:
 addl $4,%edi
 movl (%edi),%ecx
 leal -16843009(%ecx),%eax
 notl %ecx
 andl %ecx,%eax
 testl $-2139062144,%eax
 je L10


 notl %ecx
 incl %edi

 testb %cl,%cl
 je L15
 incl %edi
 shrl $8,%ecx

 testb %cl,%cl
 je L15
 incl %edi
 shrl $8,%ecx

 testb %cl,%cl
 je L15
 incl %edi



L15:
 subl %edx,%edi
 leal -1(%edi),%eax

 leal -4(%ebp),%esp
 popl %edi
 leave
 ret
