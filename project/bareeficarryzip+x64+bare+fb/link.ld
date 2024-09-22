SECTIONS{
	. = 0x0;

	.text :{
		*(.text)
	}

	.rodata :{
		*(.rodata)
		_binary_start_ = .;
		*(.binary)
		_binary_end_ = .;
	}

	.data :{
		*(.data)
	}

	.bss :{
		*(.bss)
	}
}
