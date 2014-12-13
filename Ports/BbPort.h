//
//  BbPort.h
//  BbLang
//
//  Created by Travis Henspeter on 7/13/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

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
