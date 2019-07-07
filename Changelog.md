# Version 1.6

Fix bug where changing to a smaller capacity with resizePolicy set to false
would result in invalid entries in the buffer.

# Version 1.5

Add setResizePolicy/getResizePolicy functions that set a flag in the RingBuffer
that determines if the front or back end of the buffer is truncated when the
buffer is resized to a size smaller than the current size.

Behavior of the RingBuffer before this version is preserved by defaulting to the
previous behavior (which was always truncate the back end, preserving the front
end).

# Version 1.4

Changed default capacity to 32.
Minor additions to UnitTests.

# Version 1.3

RingBuffer::pop() now returns void.
RingBuffer::top() implemented, returns reference to top.

# Version 1.2

Fixed setting a non-empty RingBuffer to capacity size 0 not toggling 
isEmpty flag.

# Version 1.1

Implemented const access (operator[], and at()).

# Version 1.0

Basically completed RingBuffer data structure.  
Documentation will be added soon.

