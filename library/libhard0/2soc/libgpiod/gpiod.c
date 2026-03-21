#include <stdio.h>
#include <unistd.h>
#include <gpiod.h>
#include "libhard.h"

#define _volt_          hex32('v','o','l','t')
#define _pin_value_     hex32('p','v', 0, 0)
#define _pin_mode_value_ hex32('p','m','v', 0)

struct privdata {
    struct gpiod_chip* chip;
    struct gpiod_line_request** request;  // v2.x 使用 request 对象
};

void setpin_input(struct privdata* priv, int id)
{
    // 创建 line settings
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        logtoall("gpiod_line_settings_new failed\n");
        return;
    }
    
    // 设置为输入模式
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
    
    // 创建 line config
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        logtoall("gpiod_line_config_new failed\n");
        gpiod_line_settings_free(settings);
        return;
    }
    
    // 添加 line settings
    unsigned int offset = id;
    int ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
    if (ret < 0) {
        logtoall("gpiod_line_config_add_line_settings failed\n");
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        return;
    }
    
    // 创建 request config
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (req_cfg) {
        gpiod_request_config_set_consumer(req_cfg, "gpioin");
    }
    
    // 发起请求
    struct gpiod_line_request *request = gpiod_chip_request_lines(priv->chip, req_cfg, line_cfg);
    
    // 清理资源
    if (req_cfg) gpiod_request_config_free(req_cfg);
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    
    if (!request) {
        logtoall("gpiod_chip_request_lines input %d failed\n", id);
        return;
    }
    
    priv->request[id] = request;
    logtoall("setpin_input %d ok\n", id);
}

void setpin_output(struct privdata* priv, int id, int val)
{
    // 创建 line settings
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        logtoall("gpiod_line_settings_new failed\n");
        return;
    }
    
    // 设置为输出模式并设置初始值
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, val ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
    
    // 创建 line config
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        logtoall("gpiod_line_config_new failed\n");
        gpiod_line_settings_free(settings);
        return;
    }
    
    // 添加 line settings
    unsigned int offset = id;
    int ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
    if (ret < 0) {
        logtoall("gpiod_line_config_add_line_settings failed\n");
        gpiod_line_settings_free(settings);
        gpiod_line_config_free(line_cfg);
        return;
    }
    
    // 创建 request config
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (req_cfg) {
        gpiod_request_config_set_consumer(req_cfg, "gpioout");
    }
    
    // 发起请求
    struct gpiod_line_request *request = gpiod_chip_request_lines(priv->chip, req_cfg, line_cfg);
    
    // 清理资源
    if (req_cfg) gpiod_request_config_free(req_cfg);
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    
    if (!request) {
        logtoall("gpiod_chip_request_lines output %d failed\n", id);
        return;
    }
    
    priv->request[id] = request;
    logtoall("setpin_output %d ok, val=%d\n", id, val);
}

int getvalue(struct privdata* priv, int id)
{
    if (!priv) return -1;
    if (!priv->request[id]) return -2;

    enum gpiod_line_value val = gpiod_line_request_get_value(priv->request[id], id);
    
    logtoall("getvalue: gpio=%d, val=%d\n", id, val);
    return val;
}

int setvalue(struct privdata* priv, int id, int val)
{
    if (!priv) return -1;
    if (!priv->request[id]) return -2;

    enum gpiod_line_value value = val ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    int ret = gpiod_line_request_set_value(priv->request[id], id, value);
    
    logtoall("setvalue: gpio=%d, val=%d, ret=%d\n", id, val, ret);
    return 0;
}

int gpio_takeby(struct halfrel* st, int sp, p64 arg, int cmd, void* buf, int len)
{
    logtoall("@gpio_takeby:%p,%x,%p,%x\n", arg, cmd, buf, len);
    return 0;
}

int gpio_giveby(struct halfrel* st, int sp, p64 arg, int cmd, void* buf, int len)
{
    logtoall("@gpio_giveby:%p,%x,%p,%x\n", arg, cmd, buf, len);
    return 0;
}

int gpio_detach(struct halfrel* st, struct halfrel* peer)
{
    return 0;
}

int gpio_attach(struct halfrel* st, struct halfrel* peer)
{
    return 0;
}

int gpio_read(_obj* obj, void* foot, p64 arg, int cmd, u8* buf, int len)
{
    struct privdata* priv = (void*)obj->priv_256b;

    switch (cmd) {
    case _pin_value_:
        for (int j = 0; j < len; j++) {
            buf[j*2 + 1] = getvalue(priv, buf[j*2 + 0]);
        }
        break;
    }
    return 0;
}

int gpio_write(_obj* obj, void* foot, p64 arg, int cmd, u8* buf, int len)
{
    struct privdata* priv = (void*)obj->priv_256b;

    int pin, mode, value;
    switch (cmd) {
    case _pin_value_:
        for (int j = 0; j < len; j++) {
            pin = buf[j*2 + 0];
            value = buf[j*2 + 1];
            logtoall("pin=%d,value=%d\n", pin, value);
            setvalue(priv, pin, value);
        }
        break;
    case _pin_mode_value_:
        for (int j = 0; j < len; j++) {
            pin = buf[j*3 + 0];
            mode = buf[j*3 + 1];
            value = buf[j*3 + 2];
            switch (mode) {
            case 'i':
                setpin_input(priv, pin);
                break;
            case 'o':
                setpin_output(priv, pin, value);
                break;
            }
        }
        break;
    }
    return 0;
}

int gpio_delete(_obj* obj, void* arg)
{
    struct privdata* priv = (void*)obj->priv_256b;
    if (priv->request) {
        for (int j = 0; j < 100; j++) {
            if (!priv->request[j]) continue;
            logtoall("gpio_delete: releasing request pin=%d, request=%p\n", j, priv->request[j]);
            gpiod_line_request_release(priv->request[j]);
            priv->request[j] = NULL;
        }
    }
    if (priv->chip) {
        gpiod_chip_close(priv->chip);
        priv->chip = NULL;
    }
    return 0;
}

int gpio_create(_obj* obj, void* arg, int argc, void** argv)
{
    char* path = arg;
    logtoall("gpio_create: path=%s\n", path);

    struct gpiod_chip* gpiochip = gpiod_chip_open(path);
    if (!gpiochip) {
        logtoall("gpiod_chip_open failed: %s\n", path);
        return -1;
    }

    struct privdata* priv = (void*)obj->priv_256b;
    priv->chip = gpiochip;
    priv->request = memoryalloc(0x1000, 0);
    
    // 初始化所有指针为 NULL
    for (int i = 0; i < 100; i++) {
        priv->request[i] = NULL;
    }
    
    logtoall("gpio_create: chip=%p, request array=%p\n", priv->chip, priv->request);
    return 0;
}

int gpio_free(_obj* obj)
{
    return 0;
}

_obj* gpio_alloc(u64 type, u8* buf)
{
    return device_alloc_fromtype(type);
}
