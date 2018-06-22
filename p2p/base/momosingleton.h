//
//  MSingleton.hpp
//  ProxyPlayer
//
//  Created by guowei on 21/08/2017.
//  Copyright © 2017 MOMO. All rights reserved.
//

#ifndef MomoSingleton_hpp
#define MomoSingleton_hpp

#include <stdio.h>
#include <pthread.h>

template <typename T>
class MomoSingleton 
{
public:
    static T* getInstance() {
        pthread_once(&sOnce, init);
        return sIntance;
    }
    
private:
    
    MomoSingleton() {
        
    }
    
    ~MomoSingleton() {
        
    }
    
    static void init() {
        sIntance = new T();
    }
    
    static pthread_once_t sOnce;
    static T*  sIntance;
};

template <typename T>
pthread_once_t MomoSingleton<T>::sOnce = PTHREAD_ONCE_INIT;

template <typename T>
T* MomoSingleton<T>::sIntance = NULL;

#endif /* MSingleton_hpp */
