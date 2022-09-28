 .global memcmp
      

memcmp:

 pushl %ebp
 movl %esp,%ebp
 subl $16,%esp
 pushl %ebx
 pushl %edi
 pushl %esi
 movl 8(%ebp),%edi
 movl 12(%ebp),%esi
 movl 16(%ebp),%ecx
 cld



 xorl %eax,%eax
 testl %ecx,%ecx
 jz L4





 movl %edi,%eax
 orl %esi,%eax
 testb $3,%al
 jne BYTECMP
 movl %ecx,%ebx
 shrl $2,%ecx
 repz
 cmpsl
 jz L5
 subl $4,%esi
 subl $4,%edi
 movl $4,%ecx
 jmp BYTECMP
L5:
 andl $3,%ebx
 movl %ebx,%ecx



BYTECMP:
 repz
 cmpsb


L3:
 xorl %edx,%edx
 movb -1(%esi),%dl
 xorl %eax,%eax
 movb -1(%edi),%al
 subl %edx,%eax

L4:
 leal -28(%ebp),%esp
 popl %esi
 popl %edi
 popl %ebx
 leave
 ret
