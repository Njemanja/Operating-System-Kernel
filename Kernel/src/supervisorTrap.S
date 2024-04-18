.extern _ZN5Riscv20handleSupervisorTrapEv
.extern _ZN5Riscv21handleSupervisorTrapTEv
.extern _ZN5Riscv21handleSupervisorTrapCEv



.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
_ZN5Riscv14supervisorTrapEv:
    j syscallroutine
    j timerroutine
    sret
    sret
    sret
    sret
    sret
    sret
    sret
    j consoleroutine
    syscallroutine:
    	# push all registers to stack
        addi sp, sp, -256
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
        .endr

        call _ZN5Riscv20handleSupervisorTrapEv

        # pop all registers from stack
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
        .endr
        addi sp, sp, 256
        sret
    consoleroutine:
        	# push all registers to stack
            addi sp, sp, -256
            .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
            sd x\index, \index * 8(sp)
            .endr

            call _ZN5Riscv21handleSupervisorTrapCEv

            # pop all registers from stack
            .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
            ld x\index, \index * 8(sp)
            .endr
            addi sp, sp, 256
            sret
    timerroutine:
            	# push all registers to stack
                addi sp, sp, -256
                .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
                sd x\index, \index * 8(sp)
                .endr

                call _ZN5Riscv21handleSupervisorTrapTEv

                # pop all registers from stack
                .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
                ld x\index, \index * 8(sp)
                .endr
                addi sp, sp, 256
                sret


