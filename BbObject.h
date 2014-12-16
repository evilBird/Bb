//
//  BbPort.h
//  BbLang
//
//  Created by Travis Henspeter on 7/13/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BbBang : NSObject
//BbBang: a generic event

+ (BbBang *)bang;

@property (nonatomic,readonly)NSDate *timeStamp;

- (NSString *)uniqueId;

@end

typedef NS_ENUM(NSInteger, BbPortConnectionStatus) {
    BbPortConnectionStatusNotConnected = 0,
    BbPortConnectionStatusConnected = 1,
    BbPortConnectionStatusTypeMismatch = 2
};

@protocol BbPortDelegate

- (void)portReceivedBang:(id)sender;

@end

@interface BbPort : NSObject 

//Current value
@property (nonatomic)id value;
//Human readable name
@property (nonatomic,strong) NSString *name;
//Unique identifer for a specific port
@property (nonatomic,strong) NSString *portId;
//Ports get assigned the object id of the BbObject instance to which they belong
@property (nonatomic,assign) NSString *objectId;
//Indicates whether the port is currently accepting input
@property (nonatomic,getter = isOpen) BOOL open;
//Keep references to observed ports
@property (nonatomic,strong) NSMutableSet *observedPorts;
//Connection status
@property (nonatomic)BbPortConnectionStatus connectionStatus;
//Port delegate 
@property (nonatomic,weak)id<BbPortDelegate>delegate;
@property (nonatomic,strong)BbPort *forwardPort;

- (instancetype)init;
- (void)observePort:(BbPort *)port;
- (void)stopObservingPort:(BbPort *)port;
- (void)forwardToPort:(BbPort *)port;
- (void)removeForwardPort:(BbPort *)port;
- (NSString *)notificationName;

@end

//
//  BbInlet.h
//  BbLang
//
//  Created by Travis Henspeter on 7/12/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

@interface BbInlet : BbPort

@property (nonatomic,getter = isHot) BOOL hot;

- (instancetype)initHot;
- (instancetype)initCold;

// pass value to inlet
- (void)input:(id)value;
- (void)handleInput:(id)input;
- (BOOL)typeOk:(id)value;

@end

//
//  BbOutlet.h
//  BbLang
//
//  Created by Travis Henspeter on 7/12/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

@class BbObject,BbOutlet;
typedef void (^BbObjectOutputBlock)(BbObject *object, BbOutlet *outlet);

@interface BbOutlet : BbPort

@property (nonatomic,strong)BbObjectOutputBlock outputBlock;

- (void)connectToInlet:(BbInlet *)inlet;
- (void)disconnectFromInlet:(BbInlet *)inlet;
- (void)output:(id)value;
- (void)handleOutput:(id)output;
- (BOOL)typeOK:(id)value;

@end

//
//  BbObject.h
//  BbLang
//
//  Created by Travis Henspeter on 7/13/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

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