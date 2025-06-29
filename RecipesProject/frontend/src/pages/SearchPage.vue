<template>
  <div class="container-fluid py-4">
    <div class="row justify-content-center">
      <div class="col-lg-10">
        <!-- Page Header -->
        <div class="text-center mb-4">
          <h1 class="display-4 text-primary">
            <i class="bi bi-search"></i> Recipe Search
          </h1>
          <p class="lead text-muted">Discover new and exciting recipes</p>
        </div>

        <!-- Search Form Component -->
        <SearchForm
          v-model:searchQuery="searchQuery"
          v-model:resultsCount="resultsCount"
          v-model:selectedCuisine="selectedCuisine"
          v-model:selectedDiet="selectedDiet"
          v-model:selectedIntolerance="selectedIntolerance"
          @search="performSearch"
          @validation-error="showError"
        />

        <!-- Last Search Display -->
        <div class="alert alert-info" v-if="lastSearch && !hasSearched">
          <div class="d-flex justify-content-between align-items-center">
            <div>
              <h5 class="alert-heading">
                <i class="bi bi-clock-history"></i> Your last search:
              </h5>
              <p class="mb-0">"{{ lastSearch.query }}" - {{ lastSearch.resultsCount }} results</p>
            </div>
            <button @click="loadLastSearch" class="btn btn-outline-primary">
              <i class="bi bi-arrow-clockwise"></i> Load last search
            </button>
          </div>
        </div>

        <!-- Search Results Component -->
        <SearchResults
          :results="searchResults"
          :loading="loading"
          :has-searched="hasSearched"
          @recipe-clicked="viewRecipe"
          @sort-changed="handleSortChange"
        />
      </div>
    </div>
  </div>
</template>

<script>
import SearchForm from '@/components/SearchForm.vue'
import SearchResults from '@/components/SearchResults.vue'
import axios from 'axios'

export default {
  name: 'SearchPage',
  components: {
    SearchForm,
    SearchResults
  },
  data() {
    return {
      searchQuery: '',
      resultsCount: 5,
      selectedCuisine: '',
      selectedDiet: '',
      selectedIntolerance: '',
      searchResults: [],
      loading: false,
      hasSearched: false,
      lastSearch: null
    }
  },
  mounted() {
    this.loadLastSearchFromStorage()
  },
  methods: {
    async performSearch(searchParams) {
      this.loading = true
      this.hasSearched = true
      try {
        // קריאה אמיתית ל-backend
        const params = {
          query: searchParams.query,
          cuisine: searchParams.cuisine,
          diet: searchParams.diet,
          intolerances: searchParams.intolerance,
          number: searchParams.resultsCount
        }
        const response = await axios.get('/recipes/search', { params })
        this.searchResults = response.data
        this.saveLastSearch(searchParams)
        this.showSuccess(`Found ${this.searchResults.length} recipes!`)
      } catch (error) {
        this.showError('An error occurred during search. Please try again.')
        console.error('Search error:', error)
      } finally {
        this.loading = false
      }
    },
    
    handleSortChange(sortBy) {
      // Sort is handled in the SearchResults component
      console.log('Sort changed to:', sortBy)
    },
    
    viewRecipe(recipeID) {
      this.$router.push(`/recipes/fullview/${recipeID}`)
    },
    
    saveLastSearch(searchParams) {
      const lastSearch = {
        query: searchParams.query,
        resultsCount: this.searchResults.length,
        timestamp: Date.now()
      }
      localStorage.setItem('lastSearch', JSON.stringify(lastSearch))
    },
    
    loadLastSearchFromStorage() {
      const saved = localStorage.getItem('lastSearch')
      if (saved) {
        this.lastSearch = JSON.parse(saved)
      }
    },
    
    loadLastSearch() {
      if (this.lastSearch) {
        this.searchQuery = this.lastSearch.query
        this.performSearch({
          query: this.lastSearch.query,
          resultsCount: this.resultsCount,
          cuisine: this.selectedCuisine,
          diet: this.selectedDiet,
          intolerance: this.selectedIntolerance
        })
      }
    },
    
    showSuccess(message) {
      this.toast('Success', message, 'success')
    },
    
    showError(message) {
      this.toast('Error', message, 'danger')
    },
    
    toast(title, content, variant = null) {
      if (window.toast) {
        window.toast(title, content, variant)
      }
    }
  }
}
</script>

<style scoped>
/* Additional custom styles if needed */
</style>
  