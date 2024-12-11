// insuranceClaimsModule.js

// Utility Service: Modular utility functions for claims processing
class UtilityService {
    static calculateApprovedAmount(claimAmount, deductible) {
        return Math.max(0, claimAmount - deductible);
    }

    static logError(error) {
        console.error(`[Error]: ${error.message}`);
    }
}

// ClaimValidator: Handles validation of claims
class ClaimValidator {
    static validateClaim(claim) {
        if (!claim.policyNumber || typeof claim.policyNumber !== 'string') {
            throw new Error("Invalid Claim: Policy number is required and must be a string.");
        }
        if (!claim.claimAmount || claim.claimAmount <= 0) {
            throw new Error("Invalid Claim: Claim amount must be greater than zero.");
        }
        return true;
    }
}

// RuleEngine: Configuration-driven validation rules
class RuleEngine {
    static applyRules(claim, rules) {
        if (claim.claimAmount > rules.maxClaimAmount) {
            throw new Error("Claim amount exceeds the allowed maximum.");
        }
        return true;
    }
}

// ClaimProcessor: Processes claims using a modular approach
class ClaimProcessor {
    constructor(rules) {
        this.rules = rules;
    }

    processClaim(claim) {
        try {
            // Validate the claim
            ClaimValidator.validateClaim(claim);
            RuleEngine.applyRules(claim, this.rules);

            // Process the claim
            const deductible = claim.policyDetails?.deductible || this.rules.defaultDeductible;
            const approvedAmount = UtilityService.calculateApprovedAmount(claim.claimAmount, deductible);

            return {
                ...claim,
                approvedAmount,
                status: "Processed",
            };
        } catch (error) {
            UtilityService.logError(error);
            return {
                ...claim,
                status: "Failed",
                error: error.message,
            };
        }
    }
}

// Asynchronous Queue Processor: Simulates handling high claim volumes
class AsyncQueueProcessor {
    constructor(processor) {
        this.processor = processor;
        this.queue = [];
    }

    addToQueue(claim) {
        this.queue.push(claim);
    }

    async processQueue() {
        while (this.queue.length > 0) {
            const claim = this.queue.shift();
            const result = this.processor.processClaim(claim);
            console.log("Processed Claim:", result);
            await new Promise((resolve) => setTimeout(resolve, 100)); // Simulating async delay
        }
    }
}

// Configuration Rules
const claimProcessingRules = {
    maxClaimAmount: 50000,
    defaultDeductible: 500,
};

// Example Claims
const claims = [
    { policyNumber: "POL123", claimAmount: 45000, policyDetails: { deductible: 1000 } },
    { policyNumber: "POL124", claimAmount: 60000 }, // Exceeds max claim amount
    { policyNumber: "POL125", claimAmount: 0 }, // Invalid claim amount
];

// Initialize Processor and Queue
const claimProcessor = new ClaimProcessor(claimProcessingRules);
const asyncQueue = new AsyncQueueProcessor(claimProcessor);

// Add Claims to Queue
claims.forEach((claim) => asyncQueue.addToQueue(claim));

// Process Claims in Queue
(async () => {
    await asyncQueue.processQueue();
})();



// insuranceClaimsModule.js

// Utility Service: Modular utility functions for claims processing
// class UtilityService {
//     static calculateApprovedAmount(claimAmount, deductible) {
//         return claimAmount - deductible; // Issue: Missing Math.max to prevent negative values
//     }

//     static logError(error) {
//         console.log(error.message); // Issue: No structured error logging
//     }
// }

// // ClaimValidator: Handles validation of claims
// class ClaimValidator {
//     static validateClaim(claim) {
//         if (!claim.policyNumber) { // Issue: Missing type check for policy number
//             throw new Error("Invalid Claim: Policy number is required.");
//         }
//         if (!claim.claimAmount) { // Issue: Does not check for zero or negative values
//             throw new Error("Invalid Claim: Claim amount is required.");
//         }
//         return true;
//     }
// }

// // RuleEngine: Configuration-driven validation rules
// class RuleEngine {
//     static applyRules(claim, rules) {
//         // Issue: No handling for undefined rules or missing configuration
//         if (claim.claimAmount > rules.maxClaimAmount) {
//             throw new Error("Claim amount exceeds the allowed maximum.");
//         }
//         return true;
//     }
// }

// // ClaimProcessor: Processes claims using a modular approach
// class ClaimProcessor {
//     constructor(rules) {
//         this.rules = rules;
//     }

//     processClaim(claim) {
//         // Issue: Missing centralized error handling mechanism
//         try {
//             // Validate the claim
//             ClaimValidator.validateClaim(claim);
//             RuleEngine.applyRules(claim, this.rules);

//             // Process the claim
//             const deductible = claim.policyDetails ? claim.policyDetails.deductible : 0; // Issue: Does not fall back to defaultDeductible
//             const approvedAmount = UtilityService.calculateApprovedAmount(claim.claimAmount, deductible);

//             return {
//                 ...claim,
//                 approvedAmount,
//                 status: "Processed",
//             };
//         } catch (error) {
//             UtilityService.logError(error);
//             return {
//                 ...claim,
//                 status: "Error", // Issue: Generic error status
//                 error: error.message,
//             };
//         }
//     }
// }

// // Asynchronous Queue Processor: Simulates handling high claim volumes
// class AsyncQueueProcessor {
//     constructor(processor) {
//         this.processor = processor;
//         this.queue = [];
//     }

//     addToQueue(claim) {
//         this.queue.push(claim);
//     }

//     async processQueue() {
//         while (this.queue.length > 0) {
//             const claim = this.queue.shift();
//             const result = this.processor.processClaim(claim);
//             console.log(result); // Issue: Insufficient logging for processed claims
//             await new Promise((resolve) => setTimeout(resolve, 100)); // Simulating async delay
//         }
//     }
// }

// // Configuration Rules
// const claimProcessingRules = {
//     maxClaimAmount: 50000, // Issue: Hardcoded rules
//     defaultDeductible: 500, // Issue: Lack of flexibility in configuration
// };

// // Example Claims
// const claims = [
//     { policyNumber: "POL123", claimAmount: 45000, policyDetails: { deductible: 1000 } },
//     { policyNumber: "POL124", claimAmount: 60000 }, // Exceeds max claim amount
//     { policyNumber: "POL125" }, // Issue: Missing claim amount
// ];

// // Initialize Processor and Queue
// const claimProcessor = new ClaimProcessor(claimProcessingRules);
// const asyncQueue = new AsyncQueueProcessor(claimProcessor);

// // Add Claims to Queue
// claims.forEach((claim) => asyncQueue.addToQueue(claim));

// // Process Claims in Queue
// (async () => {
//     await asyncQueue.processQueue();
// })();
