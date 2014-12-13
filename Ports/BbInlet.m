//
//  BSDInlet.m
//  BSDLang
//
//  Created by Travis Henspeter on 7/12/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import "BSDInlet.h"
#import "BSDOutlet.h"


@implementation BSDInlet

- (instancetype)initHot
{
    self = [super init];
    if (self) {
        _hot = YES;
    }
    return self;
}

- (instancetype)initCold
{
    return [super init];
}

- (void)input:(id)value
{
    if ([value isKindOfClass:[BSDBang class]]) {
        if (self.delegate) {
            [self.delegate portReceivedBang:self];
        }
    }else{
        
        [self handleInput:value];
    }
}

- (void)handleInput:(id)input
{
    if (self.isOpen) {
        self.value = input;
    }
}

- (BOOL)typeOk:(id)value
{
    return YES;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([object isKindOfClass:[BSDPort class]] && [self.observedPorts containsObject:object]) {
        [self input:[(BSDPort *)object value]];
    }
}


@end
