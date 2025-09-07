#ifndef _LINUX_FASTMMAP_H
#define _LINUX_FASTMMAP_H

#include <linux/mm.h>
#include <linux/jump_label.h>
struct fastmmap_ops {
    void (*init)(unsigned); 
    int (*store)(unsigned, struct file*, struct page *);
    int (*load)(unsigned, struct file*, struct page *);
};

int fastmmap_register_ops(struct fastmmap_ops *ops);

extern int __fastmmap_init(unsigned size);
extern int __fastmmap_store(struct file *file, struct page *page);
extern int __fastmmap_load(struct file *file, struct page *page);


#ifdef CONFIG_FASTMMAP
extern struct static_key_false fastmmap_enabled_key;

static inline bool fastmmap_enabled(void)
{
    return static_branch_unlikely(&fastmmap_enabled_key);
}
#else
static inline bool fastmmap_enabled(void)
{
    return false;
}
#endif

static inline int fastmmap_store(struct file *file, struct page *page)
{
    if (fastmmap_enabled())
        return __fastmmap_store(file, page);
    return -1;
}

static inline int fastmmap_load(struct file *file, struct page *page)
{
    if (fastmmap_enabled())
        return __fastmmap_load(file, page);
    return -1;
}

static inline int fastmmap_init(unsigned size)
{
#ifdef CONFIG_FASTMMAP
    return __fastmmap_init(size);
#endif
    return -1;
}
#endif