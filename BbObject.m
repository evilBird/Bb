//
//  BbObject.m
//  BbLang
//
//  Created by Travis Henspeter on 7/13/14.
//  Copyright (c) 2014 birdSound LLC. All rights reserved.
//

#import "BbObject.h"
#import "BbOutlet.h"

@implementation BbObject

#pragma mark - Public
#pragma mark - Constructors

- (instancetype)initWithArguments:(id)arguments
{
    self = [super init];
    if (self) {
        
        _observedPorts = [NSMutableSet set];
        _inlets = [NSMutableArray array];
        _outlets = [NSMutableArray array];
        _subobjects = [NSMutableArray array];
        
        _name = @"BbObject";
        _hotInlet = [self makeHotInlet];
        if (_hotInlet != nil) {
            [self addPort:_hotInlet];
        }
        _coldInlet = [self makeColdInlet];
        if (_coldInlet != nil) {
            [self addPort:_coldInlet];
        }
        
        _mainOutlet = [self makeMainOutlet];
        if (_mainOutlet != nil) {
            [self addPort:_mainOutlet];
        }

        [self setupWithArguments:arguments];
                
    }
    
    return self;
}

- (instancetype)init
{
    return [self initWithArguments:nil];
}

- (void)setOutputBlock:(BbObjectOutputBlock)outputBlock
{
    self.mainOutlet.outputBlock = outputBlock;
}

#pragma mark - overrides

// Override methods for default port config
- (BbInlet *)makeHotInlet
{
    BbInlet *hotInlet = [[BbInlet alloc]initHot];
    hotInlet.name = @"hot";
    hotInlet.objectId = [self objectId];
    hotInlet.delegate = self;
    return hotInlet;
}

- (BbInlet *)makeColdInlet
{
    BbInlet *coldInlet = [[BbInlet alloc]initCold];
    coldInlet.name = @"cold";
    coldInlet.objectId = [self objectId];
    return coldInlet;
}

- (BbOutlet *)makeMainOutlet
{
    BbOutlet *mainOutlet = [[BbOutlet alloc]init];
    mainOutlet.name = @"main";
    mainOutlet.objectId = [self objectId];
    return mainOutlet;
}

- (void)setupWithArguments:(id)arguments
{
    //override to configure before processing
}

- (void) hotInlet:(BbInlet *)inlet receivedValue:(id)value
{
    //override to get access to the hot inlet that changed
}

- (void)calculateOutput
{
    //override
}

- (void) reset
{
    //override
}

- (void)loadBang
{
    
}

- (void)test
{
    
}



#pragma mark - BbPortDelegate methods

- (void)portReceivedBang:(id)sender
{
    if ([sender isKindOfClass:[BbInlet class]]) {
        [self inletReceievedBang:sender];
    }
}

- (void)inletReceievedBang:(BbInlet *)inlet
{
    if (self.debug) {
        NSLog(@"inlet %@ receieved bang",inlet.name);
    }
}

#pragma mark - Connect to other objects/inlets
//Manage connections with specific inlets
- (void)connect:(BbInlet *)inlet
{
    [self.mainOutlet connectToInlet:inlet];
}

- (void)disconnect:(BbInlet *)inlet
{
    [self.mainOutlet disconnectFromInlet:inlet];
}

- (void) connectOutlet:(BbOutlet *)outlet toInlet:(BbInlet *)inlet
{
    [outlet connectToInlet:inlet];
}


//Manage ports
- (void)addPort:(BbPort *)port
{
    if (port && port.name) {
        if ([port isKindOfClass:[BbInlet class]]) {
            BbInlet *inlet = (BbInlet *)port;
            inlet.objectId = [self objectId];
            [self.inlets addObject:inlet];
            if (inlet.isHot) {
                [self observePort:inlet];
            }
        } else if ([port isKindOfClass:[BbOutlet class]]){
            BbOutlet *outlet = (BbOutlet *)port;
            outlet.objectId = [self objectId];
            [self.outlets addObject:outlet];
            [self observePort:outlet];
        }
    }
}

// get an inlet by name
- (BbInlet *) inletNamed:(NSString *)name
{
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"name == %@",name];
    NSArray *matches = [self.inlets filteredArrayUsingPredicate:predicate];
    if (!matches) {
        return nil;
    }
    if (matches.count == 1) {
        return matches.firstObject;
    }
    
    if (matches.count > 1) {
        NSLog(@"error: multiple inlets named %@: %@",name,matches);
        return nil;
    }
    
    return nil;
}

// get an outlet by name
- (BbOutlet *) outletNamed:(NSString *)name
{
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"name == %@",name];
    NSArray *matches = [self.outlets filteredArrayUsingPredicate:predicate];
    if (!matches) {
        return nil;
    }
    if (matches.count == 1) {
        return matches.firstObject;
    }
    
    if (matches.count > 1) {
        NSLog(@"error: multiple outlets named %@: %@",name,matches);
        return nil;
    }
    
    return nil;
}

// Unique object identifier & equality

- (NSString *)objectId
{
    if (self.assignedId != nil) {
        return self.assignedId;
    }
    
    return [NSString stringWithFormat:@"%p",self];
}

- (BOOL)isEqual:(id)object
{
    if ([object isKindOfClass:[BbObject class]]) {
        if ([[object objectId] isEqualToString:[self objectId]]) {
            return YES;
        }
    }
    
    return NO;
}

#pragma mark - Private
#pragma mark - KVO

- (void)observePort:(BbPort *)port
{
    [port addObserver:self forKeyPath:@"value" options:NSKeyValueObservingOptionNew context:nil];
    [self.observedPorts addObject:port];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"value"]) {
        
        //handle data emitted from hot inlet(s)
        if ([object isKindOfClass:[BbInlet class]]) {
            BbInlet *hotInlet = (BbInlet *)object;
            hotInlet.open = NO;
            [self hotInlet:hotInlet receivedValue:hotInlet.value];
            [self calculateOutput];
            hotInlet.open = YES;
            if (self.debug) { NSLog(@"%@",[self debugDescription]);}
            
        }else if ([object isKindOfClass:[BbOutlet class]]){
            
            //handle changes in outlet value
            BbOutlet *outlet = (BbOutlet *)object;
            if (outlet.outputBlock != NULL) {
                __weak BbObject *WEAK_SELF = self;
                outlet.outputBlock(WEAK_SELF,outlet);
            }
        }
    }
}

- (NSString *)debugDescription
{
    NSMutableString *description = [[NSMutableString alloc]init];
    [description appendFormat:@"\n\nBbOBJECT DEBUG\n"];
    [description appendFormat:@"\nobject class: %@",NSStringFromClass([self class])];
    [description appendFormat:@"\nobject name: %@",self.name];
    [description appendFormat:@"\nobject id: %@",self.objectId];
    for (BbInlet *inlet in self.inlets) {
        [description appendFormat:@"\ninlet %@ value = %@",inlet.name,inlet.value];
    }
    
    for (BbOutlet *outlet in self.outlets) {
        [description appendFormat:@"\noutlet %@ value = %@",outlet.name,outlet.value];
    }
    [description appendFormat:@"\n\n"];
    return description;
}

- (void)tearDown
{
    if (self.observedPorts.count) {
        
        for (BbPort *port in self.observedPorts) {
            [port removeObserver:self forKeyPath:@"value" context:nil];
            port.delegate = nil;
        }
    }
    
    self.observedPorts = nil;
    self.hotInlet = nil;
    self.coldInlet = nil;
    self.inlets = nil;
    self.outlets = nil;
    
}

- (void)dealloc
{
    if (self.observedPorts.count) {
        
        for (BbPort *port in self.observedPorts) {
            [port removeObserver:self forKeyPath:@"value" context:nil];
        }
    }
    self.observedPorts = nil;
    self.hotInlet = nil;
    self.coldInlet = nil;
    self.inlets = nil;
    self.outlets = nil;
    
}

@end
