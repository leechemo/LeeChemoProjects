<template>
  <div class="container-fluid py-4">
    <div class="row justify-content-center">
      <div class="col-lg-10">
        <!-- Page Header -->
        <div class="text-center mb-4">
          <h1 class="display-4 text-primary">
            <i class="bi bi-journal-text"></i> My Recipes
          </h1>
          <p class="lead text-muted">The recipes you created and developed</p>
        </div>

        <!-- Create Recipe Button -->
        <div class="text-center mb-4">
          <button @click="showCreateModal = true" class="btn btn-primary btn-lg">
            <i class="bi bi-plus-circle me-2"></i> Create New Recipe
          </button>
        </div>

        <!-- Loading State -->
        <div v-if="loading" class="text-center py-5">
          <div class="spinner-border text-primary mb-3" role="status">
            <span class="visually-hidden">Loading...</span>
          </div>
          <p class="text-muted">Loading your recipes...</p>
        </div>

        <!-- Recipes List -->
        <div v-else-if="recipes.length > 0" class="row">
          <div 
            v-for="recipe in recipes" 
            :key="recipe.recipeID" 
            class="col-md-6 col-lg-4 mb-4"
          >
            <RecipePreview :recipe="recipe" />
          </div>
        </div>

        <!-- Empty State -->
        <div v-else class="text-center py-5">
          <div class="card shadow-sm">
            <div class="card-body py-5">
              <i class="bi bi-journal-text display-1 text-muted mb-3"></i>
              <h3 class="text-muted mb-3">You don't have any recipes yet</h3>
              <p class="text-muted mb-4">
                Start creating your personal recipes and share them with the community.
              </p>
              <button @click="showCreateModal = true" class="btn btn-primary btn-lg">
                <i class="bi bi-plus-circle me-2"></i> Create Your First Recipe
              </button>
            </div>
          </div>
        </div>

        <!-- Create Recipe Modal -->
        <CreateRecipeModal 
          v-if="showCreateModal" 
          :onClose="closeModal" 
          :onSuccess="fetchMyRecipes"
        />

      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import store from '@/store';
import RecipePreview from '@/components/RecipePreview.vue';
import CreateRecipeModal from '@/components/CreateRecipeModal.vue';


export default {
  name: 'MyRecipesPage',
  components: {
    RecipePreview,
    CreateRecipeModal
  },
  data() {
    return {
      recipes: [],
      loading: true,
      showCreateModal: false
    };
  },
  created() {
    this.fetchMyRecipes();
  },
  methods: {
    async fetchMyRecipes() {
      this.loading = true;
      
      try {
        const response = await axios.get(`${store.server_domain}/user/my-recipes`, { withCredentials: true });
        this.recipes = response.data;
      } catch (error) {
        console.error('Error fetching my recipes:', error);
        this.recipes = [];
      } finally {
        this.loading = false;
      }
    },
    closeModal() {
      this.showCreateModal = false;
    }
  }
};
</script>

<style scoped>
.recipe-card {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: none;
  border-radius: 15px;
}

.recipe-card:hover {
  transform: translateY(-4px);
  box-shadow: 0 8px 25px rgba(0,0,0,0.15) !important;
}

.recipe-image {
  height: 200px;
  object-fit: cover;
  border-radius: 15px 15px 0 0;
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

.recipe-description {
  color: #666;
  line-height: 1.5;
  display: -webkit-box;
  -webkit-line-clamp: 2;
  -webkit-box-orient: vertical;
  overflow: hidden;
  font-size: 0.9rem;
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}
</style>
