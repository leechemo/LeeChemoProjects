<template>
  <div class="search-form-container">
    <div class="search-input-group mb-3">
      <input 
        v-model="localSearchQuery" 
        type="text" 
        placeholder="Search for a recipe or dish..."
        class="form-control"
        @keyup.enter="handleSearch"
      />
      <button @click="handleSearch" class="btn btn-primary">
        <i class="bi bi-search"></i> Search
      </button>
    </div>
    
    <div class="search-options mb-3">
      <div class="row">
        <div class="col-md-3">
          <label class="form-label">Number of results:</label>
          <select v-model="localResultsCount" class="form-select">
            <option value="5">5</option>
            <option value="10">10</option>
            <option value="15">15</option>
          </select>
        </div>
      </div>
    </div>

    <div class="filters-section">
      <div class="row">
        <div class="col-md-4 mb-2">
          <label class="form-label">Cuisine type:</label>
          <select v-model="localSelectedCuisine" class="form-select">
            <option value="">No filter</option>
            <option v-for="cuisine in cuisines" :key="cuisine" :value="cuisine">
              {{ cuisine }}
            </option>
          </select>
        </div>

        <div class="col-md-4 mb-2">
          <label class="form-label">Diet:</label>
          <select v-model="localSelectedDiet" class="form-select">
            <option value="">No filter</option>
            <option v-for="diet in diets" :key="diet" :value="diet">
              {{ diet }}
            </option>
          </select>
        </div>

        <div class="col-md-4 mb-2">
          <label class="form-label">Intolerance:</label>
          <select v-model="localSelectedIntolerance" class="form-select">
            <option value="">No filter</option>
            <option v-for="intolerance in intolerances" :key="intolerance" :value="intolerance">
              {{ intolerance }}
            </option>
          </select>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'SearchForm',
  props: {
    searchQuery: {
      type: String,
      default: ''
    },
    resultsCount: {
      type: Number,
      default: 5
    },
    selectedCuisine: {
      type: String,
      default: ''
    },
    selectedDiet: {
      type: String,
      default: ''
    },
    selectedIntolerance: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      cuisines: [
        'African', 'American', 'British', 'Cajun', 'Caribbean', 'Chinese', 
        'Eastern European', 'European', 'French', 'German', 'Greek', 'Indian', 
        'Irish', 'Italian', 'Japanese', 'Jewish', 'Korean', 'Latin American', 
        'Mediterranean', 'Mexican', 'Middle Eastern', 'Nordic', 'Southern', 
        'Spanish', 'Thai', 'Vietnamese'
      ],
      diets: [
        'Gluten Free', 'Ketogenic', 'Vegetarian', 'Lacto-Vegetarian', 
        'Ovo-Vegetarian', 'Vegan', 'Pescetarian', 'Paleo', 'Primal', 'Low FODMAP', 
        'Whole30'
      ],
      intolerances: [
        'Dairy', 'Egg', 'Gluten', 'Grain', 'Peanut', 'Seafood', 'Sesame', 
        'Shellfish', 'Soy', 'Sulfite', 'Tree Nut', 'Wheat'
      ]
    }
  },
  computed: {
    localSearchQuery: {
      get() {
        return this.searchQuery
      },
      set(value) {
        this.$emit('update:searchQuery', value)
      }
    },
    localResultsCount: {
      get() {
        return this.resultsCount
      },
      set(value) {
        this.$emit('update:resultsCount', parseInt(value))
      }
    },
    localSelectedCuisine: {
      get() {
        return this.selectedCuisine
      },
      set(value) {
        this.$emit('update:selectedCuisine', value)
      }
    },
    localSelectedDiet: {
      get() {
        return this.selectedDiet
      },
      set(value) {
        this.$emit('update:selectedDiet', value)
      }
    },
    localSelectedIntolerance: {
      get() {
        return this.selectedIntolerance
      },
      set(value) {
        this.$emit('update:selectedIntolerance', value)
      }
    }
  },
  methods: {
    handleSearch() {
      if (!this.localSearchQuery.trim()) {
        this.$emit('validation-error', 'Please enter a search term')
        return
      }
      
      this.$emit('search', {
        query: this.localSearchQuery,
        resultsCount: this.localResultsCount,
        cuisine: this.localSelectedCuisine,
        diet: this.localSelectedDiet,
        intolerance: this.localSelectedIntolerance
      })
    }
  }
}
</script>

<style scoped>
.search-form-container {
  background: #f8f9fa;
  padding: 2rem;
  border-radius: 12px;
  margin-bottom: 2rem;
}

.search-input-group {
  display: flex;
  gap: 1rem;
}

.search-input-group .form-control {
  flex: 1;
}

.filters-section {
  margin-top: 1.5rem;
}

@media (max-width: 768px) {
  .search-input-group {
    flex-direction: column;
  }
}
</style> 