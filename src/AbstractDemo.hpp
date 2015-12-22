#ifndef _ABSTRACT_DEMO_H_
#define _ABSTRACT_DEMO_H_

class AbstractDemo
{
public:
    virtual init() = 0;
    virtual update() = 0;
    virtual render() = 0;
    virtual shutdown() = 0;
}

#endif /* _ABSTRACT_DEMO_H_ */
