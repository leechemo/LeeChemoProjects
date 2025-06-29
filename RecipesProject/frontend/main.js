const corsOptions = {
  origin: ["http://localhost:3000", "http://localhost:8082"], 
  credentials: true
};

app.use(cors(corsOptions)); 