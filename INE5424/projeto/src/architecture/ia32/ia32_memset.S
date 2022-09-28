 .global memset
      

memset:

 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 movl 8(%ebp),%edi
 movzbl 12(%ebp),%eax
 movl 16(%ebp),%ecx
 cld



 cmpl $16,%ecx
 jbe .L19
 testl $7,%edi
 je .L10



 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx


.L10:
 movb %al,%ah
 movl %eax,%edx
 sall $16,%edx
 orl %edx,%eax

 movl %ecx,%edx
 shrl $2,%ecx
 andl $3,%edx
 rep
 stosl
 movl %edx,%ecx


.L19:
 rep
 stosb

 movl 8(%ebp),%eax

 leal -4(%ebp),%esp
 popl %edi
 leave
 ret
