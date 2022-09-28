 .global memcpy
      

memcpy:

 pushl %ebp
 movl %esp,%ebp
 pushl %esi
 pushl %edi
 pushl %ebx
 movl 8(%ebp),%edi
 movl 16(%ebp),%ecx
 movl 12(%ebp),%esi
 cld



 cmpl $8,%ecx
 jbe .L3



 movl %edi,%edx
 movl %ecx,%ebx
 andl $3,%edx
 jz .L11
 movl $4,%ecx
 subl %edx,%ecx
 andl $3,%ecx
 subl %ecx,%ebx
 rep
 movsb

 mov %ebx,%ecx



.L11:
 shrl $2,%ecx
 .p2align 2
 rep
 movsl

 movl %ebx,%ecx
 andl $3,%ecx





.L3:
 rep
 movsb
.L5:
 movl 8(%ebp),%eax

 leal -12(%ebp),%esp
 popl %ebx
 popl %edi
 popl %esi
 leave
 ret
