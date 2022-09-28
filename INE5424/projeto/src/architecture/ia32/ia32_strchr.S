 .global strchr
      

strchr:

 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 pushl %ebx
 xorl %ebx,%ebx
 movl 8(%ebp),%edi
 addb 12(%ebp),%bl



 je L25


 test $3,%edi
 je L5
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L14
 cmpb %bl,%cl
 je L19
 incl %edi

 test $3,%edi
 je L5
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L14
 cmpb %bl,%cl
 je L19
 incl %edi

 test $3,%edi
 je L5
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L14
 cmpb %bl,%cl
 je L19
 incl %edi


L5:
 movl %ebx,%ecx
 sall $8,%ebx
 subl $4,%edi
 orl %ecx,%ebx
 movl %ebx,%edx
 sall $16,%ebx
 orl %edx,%ebx


 .p2align 4,,7
L10:
 addl $4,%edi
 movl (%edi),%ecx
 leal -16843009(%ecx),%edx
 movl %ecx,%eax
 notl %eax
 andl %eax,%edx
 testl $-2139062144,%edx
 jne L9

 xorl %ebx,%ecx
 leal -16843009(%ecx),%edx
 notl %ecx
 andl %ecx,%edx
 testl $-2139062144,%edx
 je L10



L9:
 leal -1(%edi),%eax
 .p2align 4,,7
L15:
 incl %eax
 movb (%eax),%dl
 testb %dl,%dl
 je L14
 cmpb %bl,%dl
 jne L15

L14:

 cmpb %bl,(%eax)
 je L19
 xorl %eax,%eax

L19:
 leal -8(%ebp),%esp
 popl %ebx
 popl %edi
 leave
 ret
# 129 "strchr.S"
L25:

 test $3,%edi
 je L26
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L19
 incl %edi

 test $3,%edi
 je L26
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L19
 incl %edi

 test $3,%edi
 je L26
 movl %edi,%eax
 movb (%eax),%cl
 testb %cl,%cl
 je L19
 incl %edi

L26:
 subl $4,%edi


 .p2align 4,,7
L27:
 addl $4,%edi
 movl (%edi),%ecx
 leal -16843009(%ecx),%edx
 movl %ecx,%eax
 notl %eax
 andl %eax,%edx
 testl $-2139062144,%edx
 je L27

 jmp L9
