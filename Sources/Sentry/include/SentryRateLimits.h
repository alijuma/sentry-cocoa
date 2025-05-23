#import "SentryDataCategory.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Parses HTTP responses from the Sentry server for rate limits.
 * @discussion When a rate limit is reached, the SDK should stop data transmission
 * until the rate limit has expired.
 */
NS_SWIFT_NAME(RateLimits)
@protocol SentryRateLimits <NSObject>

/**
 * Check if a data category has reached a rate limit.
 * @param category the type e.g. event, error, session, transaction, etc.
 * @return @c YES if limit is reached, @c NO otherwise.
 */
- (BOOL)isRateLimitActive:(SentryDataCategory)category;

/**
 * Should be called for each HTTP response of the Sentry server. It checks the response for any
 * communicated rate limits.
 * @param response The response from the server
 */
- (void)update:(NSHTTPURLResponse *)response;

@end

NS_ASSUME_NONNULL_END
