#pragma  once
#define  INIT_SIMPLE_PROPERTY(type, propertyName, funcName) \
    protected: type (propertyName); \
    public: virtual type get##funcName(void) {return (propertyName);}; \
    public: virtual void set##funcName(type var) { propertyName = var; } 



/*
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
*/
enum eMoveDirection
{
    MOVE_LEFT,
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN
};