//
//  BSDInlet.h
//  BSDLang
//
//  Created by Travis Henspeter on 7/12/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSDPort.h"
#import "BSDBang.h"

@interface BSDInlet : BSDPort

@property (nonatomic,getter = isHot) BOOL hot;

- (instancetype)initHot;
- (instancetype)initCold;

// pass value to inlet
- (void)input:(id)value;
- (void)handleInput:(id)input;
- (BOOL)typeOk:(id)value;

@end
