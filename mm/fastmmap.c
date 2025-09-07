
#include<linux/module.h>
#include<linux/mman.h>
#include<linux/fastmmap.h>

DEFINE_STATIC_KEY_FALSE(fastmmap_enabled_key);

static struct fastmmap_ops *fastmmap_ops __read_mostly;

int fastmmap_register_ops(struct fastmmap_ops *ops)
{
    fastmmap_ops = ops;
    static_branch_inc(&fastmmap_enabled_key);
    return 0;
}

int __fastmmap_init(unsigned int size)
{
    struct fastmmap_ops *ops = fastmmap_ops;
    if (!ops || !ops->init)
        return -1;
    ops->init(size);
    return 0;
}
EXPORT_SYMBOL(__fastmmap_init);

int __fastmmap_store(struct file *file, struct page *page)
{
    struct fastmmap_ops *ops = fastmmap_ops;
    if (!ops || !ops->store)
        return -1;
    //todo:这里进行最后决定传入的参数
    return ops->store(0, file, page);
}
EXPORT_SYMBOL(__fastmmap_store);

int __fastmmap_load(struct file *file, struct page *page)
{
    struct fastmmap_ops *ops = fastmmap_ops;
    if (!ops || !ops->load)
        return -1;
    //todo:这里进行最后决定传入的参数
    return ops->load(0, file, page);
}
EXPORT_SYMBOL(__fastmmap_load);

static int __init init_fastmmap(void)
{
    //todo:debug信息的添加
    return 0;
}

module_init(init_fastmmap);
