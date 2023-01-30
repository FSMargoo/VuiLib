; Based on VMachine
; VASM (plugin by VMachine Plat)

dat     string  $string     "Hello World!\r"
dat     string  $welcome    "It's the first command on VMachine\n"
dat     string  $subtitle1  "Local machine disk size: "
dat     string  $subtitle2  "Local machine memory size: "

dat     qword   $disk_total_size
dat     qword   $mem_total_size

init_device_info:
    call    GTds_size   $disk_total_size
    call    GTme_size   $mem_total_size
end

dat     qword   $disk_start
dat     qword   $disk_end
dat     stack   $disk_dat

read_disk_range:
    dat     qword       $cnt
    mov     $disk_start fpa
    mov     $disk_end   fpb

    compb   $disk_end   $disk_total_size    ; Compare bigger than
        call    SYSabrt     0
    compl   $disk_start 0                   ; Compare less than
        call    SYSabrt     1

    call    CLR_stk     $disk_dat           ; CLR_stk : Clear stack

    mov     $cnt        $disk_start
    dat     qword       $cnt
    dat     qword       $tmp 


    .rd_disk:
        read    disk        $tmp
        push    $disk_dat   $tmp
        mova    $cnt        1
        compn   $cnt        $disk_end        ; Compare not equal
            goto    rd_disk
    end
end

dat     qword   $mem_start
dat     qword   $mem_end
dat     stack   $mem_dat

read_mem_range:
    dat     qword       $cnt
    mov     $mem_start  fpa
    mov     $mem_end    fpb

    compb   $mem_end    $mem_total_size     ; Compare bigger than
        call    SYSabrt     2

    compl   $mem_start  0                   ; Compare less than
        call    SYSabrt     3

    mov     $cnt        $mem_start
    dat     qword       $cnt
    dat     qword       $tmp

    .rd_mem:
        read    mem         $tmp
        push    $mem_dat    $tmp
        mova    $cnt        1
        compn   $cnt        $mem_end        ; Compare not equal
        goto    rd_mem
    end
end

main:
    call    init_device_info
    ; GPCS_push ： GPU character stack push
    call    GPCS_push           $string
    call    GPCS_push           $welcome
    call    GPCS_push           $subtitle1
    call    GPCS_push           $disk_total_size
    call    GPCS_push
    call    GPCS_push           $subtitle2
    call    GPCS_push           $mem_total_size
    call    GPCS_push

    dat     qword               $cnt
    mov     $cnt                0

    label:
        compn $cnt 8
        goto label

        mova $cnt 1
    end

    ; GPCT_flush : GPU Content flush
    call    GPCT_flush
end

call main