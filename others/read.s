.global _start
_start:
.att_syntax noprefix
    # open
	mov $0x5, %eax
	push $0
	push $0x7478742e #.txt
	push $0x706d742f #/tmp
	push $0x706d742f #/tmp
	mov %esp, %ebx
	mov $0, %edx
	mov $0, %ecx
	int $0x80
	#sendfile
#	mov $1, %ebx
#	mov %eax, %ecx
#	mov $0, %edx
#	sub $0x8, %esi
#	mov $187,%eax
#	int $0x80
	#read
	sub $0x10, %esp
	mov %eax,%ebx
	mov $3, %eax
	mov %esp, %ecx
	mov $8, %edx
	int $0x80
	#write
	mov $4, %eax
	mov $1,%ebx
	mov %esp, %ecx
	mov $8, %edx
	int $0x80

	#exit
	mov $1,%eax
	mov $0,%ebx
	int $0x80


# break _start