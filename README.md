Stash
=====

I've often read that instead of using malloc all over your C code it is preferable to allocated a large block of memory once and to draw from that as needed. I have always been interested in this idea so I decided to implement a simple "stash" library that would do just that.

The stash is a data structure that contains a zero-initialize array of bytes and an array of "entries" that reflects which chunks of those bytes are currently in use.

API
---

`struct stash * init_stash()`

This method returns a stash object ready for use.

`void * store_in_stash(void * object, size_t size, struct stash * s)`

This method takes a pointer to an object declared locally, copies it into the stash, and returns a pointer to the block of memory containing the stashed object. If there is a block of memory of appropriate size that was previously used but has since been freed this function will copy the new object into that that block, otherwise it will create a new "entry" in the stash.

 `bool free_object(struct stash * s, void * object)`

 This method takes a pointer to an object that has been stored in the stash and frees the entry to that block of memory in the stash. If the entry is at the end of the list it will be removed, otherwise it will be freed "in place" allowing other objects of the same size to re-use the block.
