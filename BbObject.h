//
//  BbObject.h
//  BbLang
//
//  Created by Travis Henspeter on 7/13/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BbInlet.h"
#import "BbOutlet.h"

@class BbObject;

//The abstract super class for all BbObjects. Encapsulates input/output behavior
@interface BbObject : NSObject <BbPortDelegate>

// Human-readable name
@property (nonatomic,strong) NSString *name;
// Unique per-object identifier
@property (nonatomic,readonly) NSString *objectId;

// Default port configuration
@property (nonatomic,strong) BbInlet *hotInlet;
@property (nonatomic,strong) BbInlet *coldInlet;
@property (nonatomic,strong) BbOutlet *mainOutlet;

// Override methods for default port config
- (BbInlet *)makeHotInlet;
- (BbInlet *)makeColdInlet;
- (BbOutlet *)makeMainOutlet;

// Keep a collection of additional ports
@property (nonatomic,strong) NSMutableArray *inlets;
@property (nonatomic,strong) NSMutableArray *outlets;

// Keep a reference to observed ports for managing KVO subscriptions
@property (nonatomic,strong) NSMutableSet *observedPorts;

// Optionally keep an array or dictionary of sub-objects
@property (nonatomic,strong) NSMutableArray *subobjects;

// Debug an object by printing its debug description to the log
@property (nonatomic)BOOL debug;

// designated initializer
- (instancetype) initWithArguments:(id)arguments;

// optionally configure after initializing
- (void) setupWithArguments:(id)arguments;

// Add ports
- (void) addPort:(BbPort *)port;

// Get a reference to a specific port
- (BbInlet *) inletNamed:(NSString *)name;
- (BbOutlet *) outletNamed:(NSString *)name;

// Send output value
- (void) calculateOutput;

//handle input to a specific inlet
- (void) hotInlet:(BbInlet *)inlet receivedValue:(id)value;

//handle bang message in an inlet
- (void)inletReceievedBang:(BbInlet *)inlet;

//handle bang received when object's parent is loaded
- (void)loadBang;

// Optionally implement to reset an object to a specified state
- (void) tearDown;
// Compare to other objects
- (BOOL) isEqual:(id)object;
- (NSInteger)hash;

// Debug & test
- (NSString *) debugDescription;
- (void) test;

@end
