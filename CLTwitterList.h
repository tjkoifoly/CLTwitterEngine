//
//  CLTwitterList.h
//  Sedge
//
//  Created by Casey Liss on 2/3/12.
//  Copyright (c) 2012 Casey Liss. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CLTwitterEngine.h"

@class CLTwitterUser;

@interface CLTwitterList : NSObject
{
    NSDictionary *_dictionary;
}

@property (readonly) NSString *slug;
@property (readonly) NSString *name;
@property (readonly) NSString *url;
@property (readonly) NSNumber *subscribers;
@property (readonly) NSNumber *members;
@property (readonly) NSNumber *listId;
@property (readonly) NSString *mode;
@property (readonly) NSString *fullname;
@property (readonly) NSString *desc;
@property (readonly) CLTwitterUser *user;

- (id)initWithDictionary:(NSDictionary *)dictionary;

+ (void)getAllListsWithCompletionHandler:(CLArrayHandler)handler;
+ (void)getListsForUser:(NSString *)userName withCompletionHandler:(CLArrayHandler)handler;

@end
