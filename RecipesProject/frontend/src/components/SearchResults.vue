<template>
  <div class="search-results-container">
    <!-- Results Header -->
    <div class="results-header card mb-3">
      <div class="card-body">
        <div class="row align-items-center">
          <div class="col-md-6">
            <div class="results-info">
              <span v-if="loading" class="text-muted">
                <i class="bi bi-hourglass-split"></i> Searching...
              </span>
              <span v-else-if="results.length > 0" class="text-success">
                <i class="bi bi-check-circle"></i> {{ results.length }} results found
              </span>
              <span v-else class="text-warning">
                <i class="bi bi-exclamation-triangle"></i> No results found
              </span>
            </div>
          </div>
          
          <div class="col-md-6 text-end">
            <div class="sort-options" v-if="results.length > 0">
              <label class="form-label me-2">Sort by:</label>
              <select v-model="sortBy" class="form-select d-inline-block w-auto" @change="handleSort">
                <option value="popularity">Popularity</option>
                <option value="time">Preparation Time</option>
              </select>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- Loading State -->
    <div class="loading-state text-center py-5" v-if="loading">
      <div class="spinner-border text-primary mb-3" role="status">
        <span class="visually-hidden">Loading...</span>
      </div>
      <p class="text-muted">Searching for recipes...</p>
    </div>

    <!-- Results List -->
    <div class="results-list" v-else-if="results.length > 0">
      <div class="row">
        <div 
          v-for="recipe in sortedResults" 
          :key="recipe.recipeID" 
          class="col-md-6 col-lg-4 mb-4"
        >
          <div class="recipe-card card h-100" @click="viewRecipe(recipe.recipeID)">
            <img :src="recipe.image" :alt="recipe.title" class="card-img-top recipe-image" />
            <div class="card-body">
              <h5 class="card-title recipe-title">{{ recipe.title }}</h5>
              <div class="recipe-meta mb-2">
                <span class="badge bg-primary me-2">
                  <i class="bi bi-clock"></i> {{ recipe.readyInMinutes }} min
                </span>
                <span class="badge bg-success">
                  <i class="bi bi-star"></i> {{ recipe.aggregateLikes }} likes
                </span>
              </div>
              <p class="card-text recipe-instructions">{{ recipe.instructions }}</p>
            </div>
            <div class="card-footer">
              <button class="btn btn-outline-primary btn-sm w-100">
                <i class="bi bi-eye"></i> View Recipe
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>

    <!-- No Results Message -->
    <div class="no-results text-center py-5" v-else-if="hasSearched">
      <div class="no-results-content">
        <i class="bi bi-search display-1 text-muted mb-3"></i>
        <h3 class="text-muted">No results found</h3>
        <p class="text-muted">Try changing your search terms or filters</p>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'SearchResults',
  props: {  
    results: {
      type: Array,
      default: () => []
    },
    loading: {
      type: Boolean,
      default: false
    },
    hasSearched: {
      type: Boolean,
      default: false
    }
  },
  data() {
    return {
      sortBy: 'popularity'
    }
  },
  mounted() {
    console.log("Search Results:", this.results);
  },
  computed: {
    sortedResults() {
      if (this.sortBy === 'time') {
        return [...this.results].sort((a, b) => a.readyInMinutes - b.readyInMinutes)
      } else {
        return [...this.results].sort((a, b) => b.aggregateLikes - a.aggregateLikes)
      }
    }
  },
  methods: {
    handleSort() {
      this.$emit('sort-changed', this.sortBy)
    },
    viewRecipe(recipeId) {
      this.$emit('recipe-clicked', recipeId)
    }
  }
}
</script>

<style scoped>
.search-results-container {
  margin-top: 2rem;
}

.recipe-card {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: 1px solid #dee2e6;
}

.recipe-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.15);
}

.recipe-image {
  height: 200px;
  object-fit: cover;
}

.recipe-title {
  font-size: 1.1rem;
  font-weight: 600;
  color: #333;
  margin-bottom: 0.5rem;
  line-height: 1.3;
}

.recipe-meta {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}

.recipe-instructions {
  color: #666;
  line-height: 1.5;
  display: -webkit-box;
  -webkit-line-clamp: 3;
  -webkit-box-orient: vertical;
  overflow: hidden;
  font-size: 0.9rem;
}

.no-results-content {
  max-width: 400px;
  margin: 0 auto;
}

.sort-options {
  display: flex;
  align-items: center;
  justify-content: flex-end;
}

@media (max-width: 768px) {
  .sort-options {
    justify-content: flex-start;
    margin-top: 1rem;
  }
  
  .results-header .card-body .row {
    text-align: center;
  }
}
</style> 