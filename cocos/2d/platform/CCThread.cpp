/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCThread.h"

NS_CC_BEGIN

// iOS and Mac already has a Thread.mm
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

std::list<std::function<void(void)>>* ThreadHelper::_callbackList = new std::list<std::function<void(void)>>();
std::mutex* ThreadHelper::_mutex = new std::mutex;
long ThreadHelper::_callbackNumberPerFrame = 5;


void* ThreadHelper::createAutoreleasePool()
{
    return nullptr;
}

void ThreadHelper::releaseAutoreleasePool(void* autoreleasePool)
{
    
}

void ThreadHelper::runOnGLThread(std::function<void(void)> f)
{
    // Insert call back function
    _mutex->lock();
    _callbackList->push_back(f);
    _mutex->unlock();
}

void ThreadHelper::doCallback()
{
    _mutex->lock();
    auto iter = _callbackList->begin();
    long i = 0;
    while (iter != _callbackList->end())
    {
        auto f = *iter;
        f();
        
        ++i;
        if (i >= _callbackNumberPerFrame)
        {
            break;
        }
        else
        {
            iter = _callbackList->erase(iter);
        }
    }
    _mutex->unlock();
}

void ThreadHelper::setCallbackNumberPerFrame(long callbackNumberPerFrame)
{
    _callbackNumberPerFrame = callbackNumberPerFrame;
}

#endif

NS_CC_END