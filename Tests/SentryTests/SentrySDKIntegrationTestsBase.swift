import Foundation
@testable import Sentry
import SentryTestUtils
import XCTest

// swiftlint:disable test_case_accessibility
// This is a base test class, so we can't keep all methods private.
class SentrySDKIntegrationTestsBase: XCTestCase {
    
    var currentDate = TestCurrentDateProvider()
    var crashWrapper: TestSentryCrashWrapper!
    var transportAdapter: TestTransportAdapter!
    
    var options: Options {
        Options()
    }
    
    override func setUp() {
        super.setUp()
        crashWrapper = TestSentryCrashWrapper.sharedInstance()
        SentryDependencyContainer.sharedInstance().crashWrapper = crashWrapper
        currentDate = TestCurrentDateProvider()
    }
    
    override func tearDown() {
        super.tearDown()
        clearTestState()
    }
    
    func givenSdkWithHub(_ options: Options? = nil, scope: Scope = Scope()) throws {
        
        let options = options ?? self.options
        let transport = TestTransport()
        self.transportAdapter = TestTransportAdapter(transports: [transport], options: options)
        let fileManager = try XCTUnwrap(SentryFileManager(options: options, dispatchQueueWrapper: TestSentryDispatchQueueWrapper()))
        let timezone = try XCTUnwrap(TimeZone(identifier: "Europe/Vienna"))
        
        let client = SentryClient(
            options: options,
            transportAdapter: transportAdapter,
            fileManager: fileManager,
            deleteOldEnvelopeItems: false,
            threadInspector: TestThreadInspector.instance,
            debugImageProvider: TestDebugImageProvider(),
            random: TestRandom(value: 1.0),
            locale: Locale(identifier: "en_US"),
            timezone: timezone
        )
        
        let hub = SentryHub(client: client, andScope: scope, andCrashWrapper: TestSentryCrashWrapper.sharedInstance(), andDispatchQueue: SentryDispatchQueueWrapper())
        
        SentrySDK.setStart(self.options)
        SentrySDK.setCurrentHub(hub)
    }
    
    func assertNoEventCaptured() {
        guard let client = SentrySDK.currentHub().getClient() as? TestClient else {
            XCTFail("Hub Client is not a `TestClient`")
            return
        }
        XCTAssertEqual(0, client.captureEventInvocations.count, "No event should be captured.")
    }
    
    func assertEventWithScopeCaptured(_ callback: (Event?, Scope?, [SentryEnvelopeItem]?) throws -> Void) throws {
        guard let client = SentrySDK.currentHub().getClient() as? TestClient else {
            XCTFail("Hub Client is not a `TestClient`")
            return
        }
        
        XCTAssertEqual(1, client.captureEventWithScopeInvocations.count, "More than one `Event` captured.")
        let capture = client.captureEventWithScopeInvocations.first
        try callback(capture?.event, capture?.scope, capture?.additionalEnvelopeItems)
    }
    
    private func advanceTime(bySeconds: TimeInterval) {
        currentDate.setDate(date: SentryDependencyContainer.sharedInstance().dateProvider.date().addingTimeInterval(bySeconds))
    }
}

// swiftlint:enable test_case_accessibility
