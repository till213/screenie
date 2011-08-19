#import <Cocoa/Cocoa.h>

#import <QtCore/QtGlobal>

#import "ASOpenCommand.h"

#define	scriptLoggingMasterSwitch	( 1 )

#if scriptLoggingMasterSwitch
#define SLOG(format,...) qDebug( "SLOG: File=%s line=%d proc=%s ", strrchr("/" __FILE__,'/')+1, __LINE__, __PRETTY_FUNCTION__, ## __VA_ARGS__ )
#else
#define SLOG(format,...)
#endif

@implementation ASOpenCommand

- (id)performDefaultImplementation {

    NSDictionary *theArguments = [self evaluatedArguments];
    NSString *theResult;

    SLOG(@"CommandWithArgs performDefaultImplementation");

            /* report the parameters */
    SLOG(@"The direct parameter is: '%@'", [self directParameter]);
    SLOG(@"The other parameters are: '%@'", theArguments);

            /* return the quoted direct parameter to show how to return a string from a command
            Here, if the optional ProseText parameter has been provided, we return that value in
            quotes, otherwise we return the direct parameter in quotes. */
    if ([theArguments objectForKey:@"ProseText"]) {
            theResult = [NSString stringWithFormat:@"'%@'", [theArguments objectForKey:@"ProseText"]];
    } else {
            theResult = [NSString stringWithFormat:@"'%@'", [self directParameter]];
    }

    return theResult;
}


@end
