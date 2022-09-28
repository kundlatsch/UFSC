 .global memchr
      

memchr:
 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 movzbl 12(%ebp),%eax
 movl 16(%ebp),%ecx
 movl 8(%ebp),%edi

 xorl %edx,%edx
 testl %ecx,%ecx
 jz L20
 testl $3,%edi
 je L5
 cmpb (%edi),%al
 je L15
 incl %edi
 decl %ecx
 je L20

 testl $3,%edi
 je L5
 cmpb (%edi),%al
 je L15
 incl %edi
 decl %ecx
 je L20

 testl $3,%edi
 je L5
 cmpb (%edi),%al
 je L15
 incl %edi
 decl %ecx
 je L20


L5:
 movb %al,%ah
 movl %eax,%edx
 sall $16,%edx
 orl %edx,%eax
 pushl %ebx

 .p2align 4,,7
L8:
 subl $4,%ecx
 jc L9
 movl (%edi),%edx
 addl $4,%edi
 xorl %eax,%edx
 leal -16843009(%edx),%ebx
 notl %edx
 andl %edx,%ebx
 testl $-2139062144,%ebx
 je L8

 subl $4,%edi

L9:
 popl %ebx
 xorl %edx,%edx
 addl $4,%ecx
 je L20


 .p2align 4,,7
L10:
 cmpb (%edi),%al
 je L15
 incl %edi
 decl %ecx
 jne L10

 xorl %edi,%edi



L15:
 decl %edx
 andl %edi,%edx
L20:
 movl %edx,%eax

 leal -4(%ebp),%esp
 popl %edi
 leave
 ret
