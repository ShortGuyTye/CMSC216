.text                           # IMPORTANT: subsequent stuff is executable
.global  set_tod_from_ports
        
## ENTRY POINT FOR REQUIRED FUNCTION
set_tod_from_ports:
        ## rdi is pointer to a tod
        movl    CLOCK_TIME_PORT(%rip), %esi     ##esi is the global var

        #CHECK IF VALID CLOCK TIME PORT
        cmpl $0, %esi   #test if global var is less than 0
        jl .ERROR1
        cmpl $1382400, %esi     #test if global var greater than 1/16th seconds in day
        jg .ERROR1

        jmp .GVSUCCEED

.ERROR1:
        #If fail, return 1
        movl $1, %eax
        ret

.GVSUCCEED:
        #Checking if 4th bit is set
        movl %esi, %edx     #copy of clocktimeport in edx
        andl $8, %edx
        cmpl $8, %edx
        jne .TOTALSECFAIL

.TOTALSECSUC:
        #If it is set, divide and add 1
        movl %esi, %eax     #new copy of clocktimeport for div
        cqto
        movl $16, %ecx      #make divisor 16
        idivl %ecx          #divide
        addl $1, %eax       #add one
        movl %eax, 0(%rdi)  #put total sec in correct place
        jmp .SETTIME

.TOTALSECFAIL:
        #If not, just divide
        movl %esi, %eax     #new copy of clocktimeport for div
        cqto
        movl $16, %ecx      #make divisor 16
        idivl %ecx          #divide
        movl %eax, 0(%rdi)  #put total sec in correct place

.SETTIME:
        #Divide to get hours
        movl %eax, %r8d     #save total secs for later
        cqto
        movl $3600, %ecx    #set divisor to 3600 for hours
        idivl %ecx
        cmpl $0, %eax       #if hours are 0
        je .SET12
        cmpl $12, %eax      #if hours are greater than 12
        jg .G12
        jmp .L12             #else
        

.SET12:
        #If hours is 0, set to 12
        movw $12, 8(%rdi)   #set to 12
        jmp .MINUTES

.G12:
        #If hours greater than 12, sutract 12
        subl $12, %eax
        movw %ax, 8(%rdi)  #set to hours - 12
        jmp .MINUTES

.L12:
        #otherwise, set hours
        movw %ax, 8(%rdi)  #set to hours

.MINUTES:
        #divide the remainder of hours to get minutes
        movl %edx, %eax     #set total sec / 3600 remainder to be divided
        cqto
        movl $60, %ecx      #set 60 to divisor
        idivl %ecx
        movw %ax, 6(%rdi)    #move answer to minutes
        movw %dx, 4(%rdi)    #move remainder to sec

        #set am pm
        cmpl $43200, %r8d   #compare #secs to when pm
        jl .LESSAM          
        movb $2, 10(%rdi)  #great than equal, set 2
        jmp .STFPRET
.LESSAM:
        movb $1, 10(%rdi)   #less than, set 1

.STFPRET:
        movl $0, %eax
        ret

        # load global variable into register
        # Check the C type of the variable
        #    char / short / int / long
        # and use one of
        #    movb / movw / movl / movq 
        # and appropriately sized destination register                                            

        ## DON'T FORGET TO RETURN FROM FUNCTIONS
### Change to definint semi-global variables used with the next function 
### via the '.data' directive
.data                           # IMPORTANT: use .data directive for data section

displayNums:                       # declare multiple ints sequentially starting at location
        .int 0b1110111            # 'my_array' for an array. Each are spaced 4 bytes from the
        .int 0b0100100            # next and can be given values using the same prefixes as 
        .int 0b1011101            # are understood by gcc.
        .int 0b1101101
        .int 0b0101110
        .int 0b1101011
        .int 0b1111011
        .int 0b0100101
        .int 0b1111111
        .int 0b1101111
        .int 0b0000000
        .int 0b0001000
        .int 0b1011011
        .int 0b0111111


## WARNING: Don't forget to switch back to .text as below
## Otherwise you may get weird permission errors when executing 

.text
.global  set_display_from_tod

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_from_tod:
        #TOTAL SEC
        movq %rdi, %rcx         #get first register of packed struct
        andl $0xFFFFFFFF, %ecx  #bitwise and to get only first int
        cmp $0, %rcx            #compare to 0
        jl .ERROR2
        #DAY SEC
        movq %rdi, %rcx         #reset register
        sar $32, %rcx           #shift right 32
        andq $0xFFFF, %rcx      #keep first 2 bytes
        cmp $0, %rcx            #compare too small
        jl .ERROR2
        cmp $59, %rcx           #also compare too big
        jg .ERROR2
        #DAY MIN
        movq %rdi, %rcx         #reset register
        sar $48, %rcx           #shift right 48
        andq $0xFFFF, %rcx      #keep first 2 bytes
        cmp $0, %rcx            #check small
        jl .ERROR2
        cmp $59, %rcx           #also compare too big
        jg .ERROR2
        #DAY HOUR
        movl %esi, %ecx         #reset register
        andl $0xFFFF, %ecx      #keep first 2 bytes
        cmp $0, %ecx            #check small
        jl .ERROR2
        cmp $12, %rcx           #also compare too big
        jg .ERROR2
        #AMPM
        movl %esi, %ecx         #reset register
        sarl $16, %ecx
        andl $0xFF, %ecx        #keep first 2 bytes
        cmp $1, %ecx            #check 1
        je .SETCLOCK
        cmp $2, %ecx            #check 2
        je .SETCLOCK
        jmp .ERROR2             #fall through to error
        
.ERROR2:
        movl $1, %eax
        ret

.SETCLOCK:
        movq %rdx, %r8         #move display
        movl $0,(%r8)          #set display to 0
        leaq displayNums(%rip), %r10

        #setting am pm
        movl %esi, %ecx         #reset register
        sarl $16, %ecx          #shift 16 right
        andl $0xFF, %ecx        #keep first 2 bytes
        cmp $1, %ecx
        jne .PM

        #it is equal to one
        movq $1, %r9
        salq $28, %r9
        orl %r9d, (%r8)
        jmp .SETHOURS
.PM:
        movq $1, %r9
        salq $29, %r9
        orl %r9d, (%r8)

.SETHOURS:
        movl %esi, %ecx         #reset register
        andl $0xFFFF, %ecx      #keep first 2 bytes
        cmp $10, %ecx            
        jge .PUTONE

        movq (%r10, %rcx, 4), %r9    #number of hours of the array of bits into r9
        salq $14, %r9           #shift left 14
        orl %r9d, (%r8)            #or it with the display
        jmp .SETMIN

.PUTONE:
        #fill in one
        movq 4(%r10), %r9       #seconded array into r9
        salq $21, %r9           #shift left 21
        orl %r9d, (%r8)            #or it with the display
        #fill in important number
        subq $10, %rcx
        movq (%r10, %rcx, 4), %r9    #number of hours of the array of bits into r9
        salq $14, %r9           #shift left 14
        orl %r9d, (%r8)            #or it with the display

.SETMIN:
        movq %rdi, %rcx         #reset register
        sar $48, %rcx           #shift right 48
        andq $0xFFFF, %rcx      #keep first 2 bytes
        cmp $10, %rcx
        jge .BOTHMIN

        #less than 10 min
        movq (%r10), %r9       #first array into r9
        salq $7, %r9           #shift left 7
        orl %r9d, (%r8)            #or it with the display
        #set second digit
        movq (%r10, %rcx, 4), %r9    #number of min of the array of bits into r9
        orl %r9d, (%r8)  
        jmp .SDFTRET

.BOTHMIN:
        #Divide minutes by 10
        movl %ecx, %eax
        cqto
        movl $10, %r11d
        idivl %r11d

        movq (%r10, %rax, 4), %r9    #number of min/10 of the array of bits into r9
        salq $7, %r9           #shift left 7
        orl %r9d, (%r8)            #or it with the display
        #Second digit for min
        movq (%r10, %rdx, 4), %r9    #number of min/10 of the array of bits into r9
        orl %r9d, (%r8)            #or it with the display

.SDFTRET:
        movl $0, %eax
        ret

.text
.global clock_update
        
## ENTRY POINT FOR REQUIRED FUNCTION
clock_update:
        subq $24, %rsp      #setting stack size
        
        #creat local var of tod
        movl $0, 0(%rsp)    #total sec
        movw $0, 4(%rsp)    #time sec
        movw $0, 6(%rsp)    #time min
        movw $0, 8(%rsp)    #time hour
        movw $0, 10(%rsp)   #ampm

        movq %rsp, %rdi     #set 1st arg to tod
        pushq %rbx          #save rbx on stack
        call set_tod_from_ports
        movl %eax, %ebx     #save return value

        #make packed struct
        movl 8(%rsp), %edi  #move total sec to first register
        movw 12(%rsp), %cx  #save time sec
        salq $32, %rcx      #shift and and to first register
        orq %rcx, %rdi

        movw 14(%rsp), %cx  #save time min
        andq $0xFFFF, %rcx
        salq $48, %rcx      #shift and and to first register
        orq %rcx, %rdi

        movq $0, %rsi
        movw 16(%rsp), %si  #move time hour to second register
        salq $4, %rsi
        sarq $4, %rsi
        movw 18(%rsp), %cx  #save time sec
        andq $0xFFFF, %rcx
        salq $16, %rcx      #shift and and to first register
        orq %rcx, %rsi

        #move display to argument 3
        leaq    CLOCK_DISPLAY_PORT(%rip), %rdx

        pushq %r12          #save r12
        call set_display_from_tod
        movl %eax, %r12d    #save return

        #check if either returned 1
        cmpl $1, %ebx
        je .ERROR3
        cmpl $1, %r12d
        je .ERROR3
        jmp .SUCCESS

.ERROR3:
#undo pushes
        popq %r12
        popq %rbx
        #undo local variables
        addq $24, %rsp
        movl $1, %eax
        ret

.SUCCESS:
        #undo pushes
        popq %r12
        popq %rbx
        #undo local variables
        addq $24, %rsp
        movl $0, %eax
        ret
