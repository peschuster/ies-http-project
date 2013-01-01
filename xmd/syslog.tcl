proc mb_mrd_w { address } {
	return 0x[ string range [mrd $address 1 w] 12  19]
}

proc mysyslog { } {
    # set bufaddr 0xc04e3c10
	# set bufaddr 0xc04e6c10
	# set bufaddr 0xc0433c10
	set bufaddr 0xc1840ba0
    set bufsize 0x00020000

    set startaddr $bufaddr
    set endaddr [expr $bufaddr + $bufsize]
	
	set fp [open "syslog_neu1.txt" w]

    puts $fp "Displaying Linux syslog buffer of $bufsize length at $bufaddr"
	
    while {$startaddr < $endaddr} {
        # Read 4 bytes of text
        set mval [ mb_mrd_w $startaddr ]

        # Display the word of text, a byte at a time
        set shift 24
        while {$shift >= 0} {
            set char [expr [expr $mval >> $shift] & 0xff]
            # NULL ? End of string.
            if {$char == 0} {
                close $fp
				return
            }
            set char [format "%c" $char]
            puts -nonewline $fp "$char"

            incr shift -8
        }

        incr startaddr 4
    }
    puts $fp ""
	close $fp
}

proc syslog_write { } {
	set fp [open "syslog_t.txt" w]
	puts $fp [ mrd 0xc04e3c10 100 w ]
	close $fp
}