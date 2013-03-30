/*
 * Basic general purpose allocator for managing special purpose memory
 * not managed by the regular kmalloc/kfree interface.
 * Uses for this includes on-device special memory, uncached memory
 * etc.
 *
 * Copyright 2005 (C) Jes Sorensen <jes@trained-monkey.org>
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/bitmap.h>
#include <linux/genalloc.h>


<<<<<<< HEAD
/* General purpose special memory pool descriptor. */
struct gen_pool {
	rwlock_t lock;			/* protects chunks list */
	struct list_head chunks;	/* list of chunks in this pool */
	unsigned order;			/* minimum allocation order */
};

/* General purpose special memory pool chunk descriptor. */
struct gen_pool_chunk {
	spinlock_t lock;		/* protects bits */
	struct list_head next_chunk;	/* next chunk in pool */
	phys_addr_t phys_addr;		/* physical starting address of memory chunk */
	unsigned long start;		/* start of memory chunk */
	unsigned long size;		/* number of bits */
	unsigned long bits[0];		/* bitmap for allocating memory chunk */
};

/**
 * gen_pool_create() - create a new special memory pool
 * @order:	Log base 2 of number of bytes each bitmap bit
 *		represents.
 * @nid:	Node id of the node the pool structure should be allocated
 *		on, or -1.  This will be also used for other allocations.
=======
/**
 * gen_pool_create - create a new special memory pool
 * @min_alloc_order: log base 2 of number of bytes each bitmap bit represents
 * @nid: node id of the node the pool structure should be allocated on, or -1
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
 *
 * Create a new special memory pool that can be used to manage special purpose
 * memory not managed by the regular kmalloc/kfree interface.
 */
<<<<<<< HEAD
struct gen_pool *__must_check gen_pool_create(unsigned order, int nid)
{
	struct gen_pool *pool;

	if (WARN_ON(order >= BITS_PER_LONG))
		return NULL;

	pool = kmalloc_node(sizeof *pool, GFP_KERNEL, nid);
	if (pool) {
		rwlock_init(&pool->lock);
		INIT_LIST_HEAD(&pool->chunks);
		pool->order = order;
=======
struct gen_pool *gen_pool_create(int min_alloc_order, int nid)
{
	struct gen_pool *pool;

	pool = kmalloc_node(sizeof(struct gen_pool), GFP_KERNEL, nid);
	if (pool != NULL) {
		rwlock_init(&pool->lock);
		INIT_LIST_HEAD(&pool->chunks);
		pool->min_alloc_order = min_alloc_order;
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
	}
	return pool;
}
EXPORT_SYMBOL(gen_pool_create);

/**
 * gen_pool_add_virt - add a new chunk of special memory to the pool
 * @pool: pool to add new memory chunk to
 * @virt: virtual starting address of memory chunk to add to pool
 * @phys: physical starting address of memory chunk to add to pool
 * @size: size in bytes of the memory chunk to add to pool
 * @nid: node id of the node the chunk structure and bitmap should be
 *       allocated on, or -1
 *
 * Add a new chunk of special memory to the specified pool.
 *
 * Returns 0 on success or a -ve errno on failure.
 */
<<<<<<< HEAD
int __must_check gen_pool_add_virt(struct gen_pool *pool, unsigned long virt, phys_addr_t phys,
		 size_t size, int nid)
{
	struct gen_pool_chunk *chunk;
	size_t nbytes;

	if (WARN_ON(!virt || virt + size < virt ||
	    (virt & ((1 << pool->order) - 1))))
		return -EINVAL;

	size = size >> pool->order;
	if (WARN_ON(!size))
		return -EINVAL;

	nbytes = sizeof *chunk + BITS_TO_LONGS(size) * sizeof *chunk->bits;
	chunk = kzalloc_node(nbytes, GFP_KERNEL, nid);
	if (!chunk)
=======
int gen_pool_add_virt(struct gen_pool *pool, unsigned long virt, phys_addr_t phys,
		 size_t size, int nid)
{
	struct gen_pool_chunk *chunk;
	int nbits = size >> pool->min_alloc_order;
	int nbytes = sizeof(struct gen_pool_chunk) +
				BITS_TO_LONGS(nbits) * sizeof(long);

	chunk = kmalloc_node(nbytes, GFP_KERNEL | __GFP_ZERO, nid);
	if (unlikely(chunk == NULL))
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
		return -ENOMEM;

	spin_lock_init(&chunk->lock);
	chunk->phys_addr = phys;
<<<<<<< HEAD
	chunk->start = virt >> pool->order;
	chunk->size  = size;
=======
	chunk->start_addr = virt;
	chunk->end_addr = virt + size;
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y

	write_lock(&pool->lock);
	list_add(&chunk->next_chunk, &pool->chunks);
	write_unlock(&pool->lock);

	return 0;
}
EXPORT_SYMBOL(gen_pool_add_virt);

/**
 * gen_pool_virt_to_phys - return the physical address of memory
 * @pool: pool to allocate from
 * @addr: starting address of memory
 *
 * Returns the physical address on success, or -1 on error.
 */
phys_addr_t gen_pool_virt_to_phys(struct gen_pool *pool, unsigned long addr)
{
	struct list_head *_chunk;
	struct gen_pool_chunk *chunk;

	read_lock(&pool->lock);
	list_for_each(_chunk, &pool->chunks) {
		chunk = list_entry(_chunk, struct gen_pool_chunk, next_chunk);

<<<<<<< HEAD
		if (addr >= chunk->start &&
		    addr < (chunk->start + chunk->size))
			return chunk->phys_addr + addr - chunk->start;
=======
		if (addr >= chunk->start_addr && addr < chunk->end_addr)
			return chunk->phys_addr + addr - chunk->start_addr;
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
	}
	read_unlock(&pool->lock);

	return -1;
}
EXPORT_SYMBOL(gen_pool_virt_to_phys);

/**
<<<<<<< HEAD
 * gen_pool_destroy() - destroy a special memory pool
 * @pool:	Pool to destroy.
=======
 * gen_pool_destroy - destroy a special memory pool
 * @pool: pool to destroy
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
 *
 * Destroy the specified special memory pool. Verifies that there are no
 * outstanding allocations.
 */
void gen_pool_destroy(struct gen_pool *pool)
{
<<<<<<< HEAD
	struct gen_pool_chunk *chunk;
	int bit;

	while (!list_empty(&pool->chunks)) {
		chunk = list_entry(pool->chunks.next, struct gen_pool_chunk,
				   next_chunk);
		list_del(&chunk->next_chunk);

		bit = find_next_bit(chunk->bits, chunk->size, 0);
		BUG_ON(bit < chunk->size);
=======
	struct list_head *_chunk, *_next_chunk;
	struct gen_pool_chunk *chunk;
	int order = pool->min_alloc_order;
	int bit, end_bit;


	list_for_each_safe(_chunk, _next_chunk, &pool->chunks) {
		chunk = list_entry(_chunk, struct gen_pool_chunk, next_chunk);
		list_del(&chunk->next_chunk);

		end_bit = (chunk->end_addr - chunk->start_addr) >> order;
		bit = find_next_bit(chunk->bits, end_bit, 0);
		BUG_ON(bit < end_bit);
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y

		kfree(chunk);
	}
	kfree(pool);
<<<<<<< HEAD
=======
	return;
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
}
EXPORT_SYMBOL(gen_pool_destroy);

/**
<<<<<<< HEAD
 * gen_pool_alloc_aligned() - allocate special memory from the pool
 * @pool:	Pool to allocate from.
 * @size:	Number of bytes to allocate from the pool.
 * @alignment_order:	Order the allocated space should be
 *			aligned to (eg. 20 means allocated space
 *			must be aligned to 1MiB).
=======
 * gen_pool_alloc - allocate special memory from the pool
 * @pool: pool to allocate from
 * @size: number of bytes to allocate from the pool
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
 *
 * Allocate the requested number of bytes from the specified pool.
 * Uses a first-fit algorithm.
 */
<<<<<<< HEAD
unsigned long __must_check
gen_pool_alloc_aligned(struct gen_pool *pool, size_t size,
		       unsigned alignment_order)
{
	unsigned long addr, align_mask = 0, flags, start;
	struct gen_pool_chunk *chunk;
=======
unsigned long gen_pool_alloc(struct gen_pool *pool, size_t size)
{
	struct list_head *_chunk;
	struct gen_pool_chunk *chunk;
	unsigned long addr, flags;
	int order = pool->min_alloc_order;
	int nbits, start_bit, end_bit;
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y

	if (size == 0)
		return 0;

<<<<<<< HEAD
	if (alignment_order > pool->order)
		align_mask = (1 << (alignment_order - pool->order)) - 1;

	size = (size + (1UL << pool->order) - 1) >> pool->order;

	read_lock(&pool->lock);
	list_for_each_entry(chunk, &pool->chunks, next_chunk) {
		if (chunk->size < size)
			continue;

		spin_lock_irqsave(&chunk->lock, flags);
		start = bitmap_find_next_zero_area_off(chunk->bits, chunk->size,
						       0, size, align_mask,
						       chunk->start);
		if (start >= chunk->size) {
=======
	nbits = (size + (1UL << order) - 1) >> order;

	read_lock(&pool->lock);
	list_for_each(_chunk, &pool->chunks) {
		chunk = list_entry(_chunk, struct gen_pool_chunk, next_chunk);

		end_bit = (chunk->end_addr - chunk->start_addr) >> order;

		spin_lock_irqsave(&chunk->lock, flags);
		start_bit = bitmap_find_next_zero_area(chunk->bits, end_bit, 0,
						nbits, 0);
		if (start_bit >= end_bit) {
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
			spin_unlock_irqrestore(&chunk->lock, flags);
			continue;
		}

<<<<<<< HEAD
		bitmap_set(chunk->bits, start, size);
		spin_unlock_irqrestore(&chunk->lock, flags);
		addr = (chunk->start + start) << pool->order;
		goto done;
	}

	addr = 0;
done:
	read_unlock(&pool->lock);
	return addr;
}
EXPORT_SYMBOL(gen_pool_alloc_aligned);

/**
 * gen_pool_free() - free allocated special memory back to the pool
 * @pool:	Pool to free to.
 * @addr:	Starting address of memory to free back to pool.
 * @size:	Size in bytes of memory to free.
=======
		addr = chunk->start_addr + ((unsigned long)start_bit << order);

		bitmap_set(chunk->bits, start_bit, nbits);
		spin_unlock_irqrestore(&chunk->lock, flags);
		read_unlock(&pool->lock);
		return addr;
	}
	read_unlock(&pool->lock);
	return 0;
}
EXPORT_SYMBOL(gen_pool_alloc);

/**
 * gen_pool_free - free allocated special memory back to the pool
 * @pool: pool to free to
 * @addr: starting address of memory to free back to pool
 * @size: size in bytes of memory to free
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
 *
 * Free previously allocated special memory back to the specified pool.
 */
void gen_pool_free(struct gen_pool *pool, unsigned long addr, size_t size)
{
<<<<<<< HEAD
	struct gen_pool_chunk *chunk;
	unsigned long flags;

	if (!size)
		return;

	addr = addr >> pool->order;
	size = (size + (1UL << pool->order) - 1) >> pool->order;

	BUG_ON(addr + size < addr);

	read_lock(&pool->lock);
	list_for_each_entry(chunk, &pool->chunks, next_chunk)
		if (addr >= chunk->start &&
		    addr + size <= chunk->start + chunk->size) {
			spin_lock_irqsave(&chunk->lock, flags);
			bitmap_clear(chunk->bits, addr - chunk->start, size);
			spin_unlock_irqrestore(&chunk->lock, flags);
			goto done;
		}
	BUG_ON(1);
done:
=======
	struct list_head *_chunk;
	struct gen_pool_chunk *chunk;
	unsigned long flags;
	int order = pool->min_alloc_order;
	int bit, nbits;

	nbits = (size + (1UL << order) - 1) >> order;

	read_lock(&pool->lock);
	list_for_each(_chunk, &pool->chunks) {
		chunk = list_entry(_chunk, struct gen_pool_chunk, next_chunk);

		if (addr >= chunk->start_addr && addr < chunk->end_addr) {
			BUG_ON(addr + size > chunk->end_addr);
			spin_lock_irqsave(&chunk->lock, flags);
			bit = (addr - chunk->start_addr) >> order;
			while (nbits--)
				__clear_bit(bit++, chunk->bits);
			spin_unlock_irqrestore(&chunk->lock, flags);
			break;
		}
	}
	BUG_ON(nbits > 0);
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
	read_unlock(&pool->lock);
}
EXPORT_SYMBOL(gen_pool_free);
