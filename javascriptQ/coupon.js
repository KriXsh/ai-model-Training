// code debugging 
// bug fixing for get api 





const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
require('dotenv').config();
const Joi = require('joi');
const { ObjectId } = require('mongodb');

const app = express();
app.use(bodyParser.json());

const MongoClient = require('mongodb').MongoClient;
let db;
const MONGO_URL = "mongodb://localhost:27017"
const MONGO_DB_NAME = "Coupons"
// Database connection
const connectDB = async () => {
    try {
        const client = new MongoClient(MONGO_URL, {
            useNewUrlParser: true,
            useUnifiedTopology: true,
        });
        await client.connect();
        db = client.db(MONGO_DB_NAME);
        console.log('Database connection established successfully');
    } catch (error) {
        console.error('Database connection failed:', error);
        throw error;
    }
};
connectDB();

// Helper Functions
const insert = async (collection, data) => {
    try {
        const lower = collection.toLowerCase();
        const result = await db.collection(lower).insertMany(data);
        return result;
    } catch (exception) {
        console.error("Exception:", exception);
        throw exception;
    }
};


const getDocById = (collection, id) => {
    return new Promise(async (resolve, reject) => {
        try {
            const lower = collection.toLowerCase();
            const result = await db.collection(lower).findOne({ _id: new ObjectId(id) });
            console.log(result);
            return resolve(result);
        } catch (exception) {
            console.error("Exception:", exception);
            return reject(exception);
        }
    });
};

const get = async (collection, filter) => {
    try {
        const lower = collection.toLowerCase();
        const result = await db.collection(lower).findOne(filter);
        return result;
    } catch (exception) {
        console.error("Exception:", exception);
        throw exception;
    }
};

// Error Mapping
const errorMapping = {
    '11000': 'Duplicate entry detected. Please check your data.',
    'exception': 'An unexpected error occurred.',
};

const standardManageError = (req, res, message, type) => {
    return res.status(400).json({
        code: 400,
        message,
        type,
    });
};

app.post('/create-coupons', async (req, res) => {
  try {
      const { error, value } = couponSchema.validate(req.body, { abortEarly: false });
      if (error) {
          const errorMessages = error.details.map((err) => err.message);
          return res.status(400).json({
              code: 400,
              message: 'Validation failed.',
              errors: errorMessages,
          });
      }

      // If validation passes, proceed with processing
      const dbPayload = {
          ...value,
          createdAt: new Date(),
      };

      console.log('Payload:', dbPayload);
      const result = await insert('coupons', [dbPayload]);

      // Return success response
      return res.status(200).json({
          code: 200,
          message: 'Coupon created successfully',
          data: result,
      });
  } catch (exception) {
      console.error(exception);
      const errorMessage = errorMapping[exception.code] ||
          'An unexpected error occurred. Please try again later.';
      return standardManageError(req, res, errorMessage, 'exception');
  }
});


app.get('/coupons/:id', async (req, res) => {
    try {
        const id = req.params.id; // Extract the id from req.params

        // Validate if id is a valid ObjectId
        if (!ObjectId.isValid(id)) {
            return standardManageError(
                req,
                res,
                'Invalid coupon ID format. Please provide a valid Coupon ID.',
                'validation'
            );
        }

        // Fetch the document by ID
        const getCoupons = await getDocById('coupons', id);

        if (!getCoupons) {
            return standardManageError(
                req,
                res,
                `Coupons not found`,
                'notFound'
            );
        }

        // Return the retrieved coupon
        return res.status(200).json({
            code: 200,
            message: 'Coupon retrieved successfully',
            data: getCoupons,
        });
    } catch (exception) {
        console.error(exception);
        const errorMessage = errorMapping[exception.code] ||
            'An unexpected error occurred. Please try again later.';
        return standardManageError(
            req,
            res,
            errorMessage,
            'exception'
        );
    }
});



const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
