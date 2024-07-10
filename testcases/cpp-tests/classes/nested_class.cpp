class outerClass
{
    public:
        static int item;

        int accessInner() {
            return item;
        }

        class innerClass
        {
            public:
                int nestedItem;

            int accessOuter() {
                return nestedItem;
            }
        };
};

// If accessOuter is associated with innerClass, LCOM4 of outerClass = 2
// If accessOuter is not associated with innerClass, LCOM4 of outerClass = 1
// Tool result: LCOM4 of outerClass = 1